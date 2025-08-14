#include "Fullscreen.hlsli"

struct OutlineData
{
    float4x4 projectionInverse;
};
ConstantBuffer<OutlineData> gOutLineData : register(b0);

// t0=SceneColor, t1=Depth(SRV: R24_UNORM_X8_TYPELESS), t2=Mask(R=mode / GBA=lineColor), t3=ObjectID(R32_UINT)
Texture2D<float4> gTexture : register(t0);
Texture2D<float> gDepthTexture : register(t1);
Texture2D<float4> gOutlineMask : register(t2);
Texture2D<uint> gObjectID : register(t3);

SamplerState gSampler : register(s0); // Linear（色）
SamplerState gSamplerPoint : register(s1); // Point（深度）

struct PSOut
{
    float4 color : SV_Target0;
};

// -------- 調整パラメータ --------
#define RADIUS        1
#define K_SILHOUETTE  1.00f
#define K_NORMAL      1.00f
#define K_ID          1.25f
#define K_COLOR       0.75f
#define EDGE_GAIN     1.10f
#define EPS_Z0        0.002f
#define EPS_Z1        0.0015f

// 0: outside, 1: Depth+Normal, 2: Depth+Normal+SceneColor
uint ClassifyMask(float m)
{
    // 量子化/色空間の差異を吸収（0.25/0.75 の緩い段階しきい値）
    return (m > 0.75f) ? 2u : (m > 0.25f) ? 1u : 0u;
}
bool InMask(uint mode)
{
    return mode >= 1u;
} // 0.5/1.0 を内側扱い
bool WantColor(uint mode)
{
    return mode >= 2u;
} // 1.0 のときだけ色境界

float3 ReconstructViewPos(float2 uv, float d01, float4x4 invP)
{
    float2 ndc = uv * 2.0 - 1.0;
    float4 v = mul(invP, float4(ndc, d01, 1.0));
    return v.xyz / v.w;
}
float3 NormalFromDepth(float2 uv, float2 stepD, float4x4 invP)
{
    float dC = gDepthTexture.SampleLevel(gSamplerPoint, uv, 0).r;
    float3 PC = ReconstructViewPos(uv, dC, invP);
    float dX = gDepthTexture.SampleLevel(gSamplerPoint, uv + float2(stepD.x, 0), 0).r;
    float dY = gDepthTexture.SampleLevel(gSamplerPoint, uv + float2(0, stepD.y), 0).r;
    float3 PX = ReconstructViewPos(uv + float2(stepD.x, 0), dX, invP);
    float3 PY = ReconstructViewPos(uv + float2(0, stepD.y), dY, invP);
    float3 n = normalize(cross(PX - PC, PY - PC));
    return any(isnan(n)) ? float3(0, 0, 1) : n;
}
float Luma(float3 c)
{
    return dot(c, float3(0.2126, 0.7152, 0.0722));
}

// UV⇄ピクセル変換（中心座標）
int2 ToPix(float2 uv, uint w, uint h)
{
    float2 st = saturate(uv) * float2(max(w, 1u), max(h, 1u)) - 0.5; // 中心基準
    int2 p = int2(round(st));
    p.x = clamp(p.x, 0, int(w) - 1);
    p.y = clamp(p.y, 0, int(h) - 1);
    return p;
}
float2 PixCenterUV(int2 p, uint w, uint h)
{
    return (float2(p) + 0.5) / float2(max(w, 1u), max(h, 1u));
}

PSOut main(VertexShaderOutput i)
{
    // 各テクスチャのサイズ
    uint cw, ch;
    gTexture.GetDimensions(cw, ch);
    uint dw, dh;
    gDepthTexture.GetDimensions(dw, dh);
    uint mw, mh;
    gOutlineMask.GetDimensions(mw, mh);
    uint iw, ih;
    gObjectID.GetDimensions(iw, ih);
    float2 stepD = 1.0 / float2(dw, dh);
    float2 stepC = 1.0 / float2(cw, ch);

    // 中心を「Mask ピクセル中心」に合わせて処理を開始
    int2 pixM_C = ToPix(i.texcoord, mw, mh);
    float2 uvC = PixCenterUV(pixM_C, mw, mh);

    float3 baseCol = gTexture.Sample(gSampler, uvC).rgb;

    // Mask を Load（非補間）で取得
    float mR = gOutlineMask.Load(int3(pixM_C, 0)).r;
    uint mC = ClassifyMask(mR);
    bool inC = InMask(mC);
    bool wantColorC = WantColor(mC);

    // Depth/Normal/ID（中心）
    float d01C = gDepthTexture.SampleLevel(gSamplerPoint, uvC, 0).r;
    float3 Pc = ReconstructViewPos(uvC, d01C, gOutLineData.projectionInverse);
    float zc = abs(Pc.z);
    float3 nC = NormalFromDepth(uvC, stepD, gOutLineData.projectionInverse);
    float lC = Luma(baseCol);

    int2 pixI_C = ToPix(uvC, iw, ih);
    uint idC = gObjectID.Load(int3(pixI_C, 0));

    // ---- (A) シルエット抽出（膨張-収縮；二値化）----
    float mDil = 0.0f, mEro = 1.0f;
    [unroll]
    for (int dy = -RADIUS; dy <= RADIUS; ++dy)
    [unroll]
        for (int dx = -RADIUS; dx <= RADIUS; ++dx)
        {
            int2 q = int2(clamp(pixM_C.x + dx, 0, int(mw) - 1),
                          clamp(pixM_C.y + dy, 0, int(mh) - 1));
            float mr = gOutlineMask.Load(int3(q, 0)).r;
            float mb = InMask(ClassifyMask(mr)) ? 1.0f : 0.0f;
            mDil = max(mDil, mb);
            mEro = min(mEro, mb);
        }
    float edgeSil = saturate(mDil - mEro);

    // 近傍評価
    float normalW = 0.0f;
    float idEdgeW = 0.0f;
    float colorW = 0.0f;

    // 線色は“可視な最手前”を採用（Mask.gba）
    float3 bestCol = inC ? gOutlineMask.Load(int3(pixM_C, 0)).gba : 0.0f;
    float bestZ = 1e30f;
    bool haveCol = inC;

    float eps = EPS_Z0 + EPS_Z1 * zc;

    const int R = RADIUS;
    [unroll]
    for (int dy = -R; dy <= R; ++dy)
    [unroll]
        for (int dx = -R; dx <= R; ++dx)
        {
            if (dx == 0 && dy == 0)
                continue;

        // 近傍座標（まずは Mask の整数ピクセルで定義）
            int2 pM = int2(clamp(pixM_C.x + dx, 0, int(mw) - 1),
                          clamp(pixM_C.y + dy, 0, int(mh) - 1));
            float2 uvN = PixCenterUV(pM, mw, mh);

            float mR_N = gOutlineMask.Load(int3(pM, 0)).r;
            uint mN = ClassifyMask(mR_N);
            bool inN = InMask(mN);
            bool wantColorN = WantColor(mN);
            bool wantColorEdge = (wantColorC || wantColorN);

        // ID は ID テクスチャのピクセル座標に変換して Load
            int2 pI = ToPix(uvN, iw, ih);
            uint idN = gObjectID.Load(int3(pI, 0));

        // Depth/Normal/Color は uvN を使ってサンプル
            float d01 = gDepthTexture.SampleLevel(gSamplerPoint, uvN, 0).r;
            float3 Pn = ReconstructViewPos(uvN, d01, gOutLineData.projectionInverse);
            float zN = abs(Pn.z);

        // (1) 法線差：対象の内側/近傍のみ
            if (inC || inN)
            {
                float3 nN = NormalFromDepth(uvN, stepD, gOutLineData.projectionInverse);
                normalW = max(normalW, 1.0f - saturate(dot(nC, nN)));
            }

        // (2) ID 境界：羽≠本体 かつ 可視側だけ
            if ((idC != idN) && (inC || inN))
            {
                if (zN <= zc + eps)
                    idEdgeW = 1.0f;
            }

        // (3) 同一ID内の色境界：1.0 指定のときだけ評価
            if (wantColorEdge && (idC == idN) && (inC || inN) && (zN <= zc + eps))
            {
                float3 cN = gTexture.Sample(gSampler, uvN).rgb;
                colorW = max(colorW, abs(Luma(cN) - lC));
            }

        // 線色：可視な最手前を採用
            if (inN)
            {
                if (zN <= zc + eps && zN < bestZ)
                {
                    bestZ = zN;
                    bestCol = gOutlineMask.Load(int3(pM, 0)).gba;
                    haveCol = true;
                }
            }
        }

    // 正規化と合成
    float normalN = saturate((normalW - 0.08f) * 6.0f);
    float colorN = saturate((colorW - 0.04f) * 30.0f);
    float edge = K_SILHOUETTE * edgeSil
               + K_NORMAL * normalN
               + K_ID * idEdgeW
               + K_COLOR * colorN; // 1.0 モード時のみ非0
    edge = saturate(edge * EDGE_GAIN);

    PSOut o;    
    o.color = (edge <= 0.0f || !haveCol) ? float4(baseCol, 1.0f)
                                         : float4(lerp(baseCol, bestCol, edge), 1.0f);
    return o;
}
