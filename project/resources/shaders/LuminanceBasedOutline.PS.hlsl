#include "Fullscreen.hlsli"

struct OutlineData
{
    float4x4 projectionInverse;
};

ConstantBuffer<OutlineData> gOutLineData : register(b0);
Texture2D<float4> gTexture : register(t0);
Texture2D<float> gDepthTexture : register(t1);
Texture2D<float4> gOutlineMask : register(t2);

SamplerState gSampler : register(s0);
SamplerState gSamplerPoint : register(s1);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

static const float PI = 3.14159265f;

float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);
}

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float Luminance(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input)
{
    // 画素サイズ
    int width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStep = float2(1.0 / (float) width, 1.0 / (float) height);

    // ▼ マスク(R)の膨張＆色(GBA)の取得
    //    R が最大だったサンプルの色を採用する簡易ルール
    const int R = 2; // 太さ：1～3くらいで調整
    float m = 0.0f;
    float3 oc = float3(0, 0, 0);

    [unroll]
    for (int dy = -R; dy <= R; ++dy)
    [unroll]
        for (int dx = -R; dx <= R; ++dx)
        {
            float2 o = float2(dx, dy) * uvStep;
            float4 ms = gOutlineMask.SampleLevel(gSamplerPoint, input.texcoord + o, 0);
            if (ms.r > m)
            { // マスクが強いサンプルを採用
                m = ms.r; // マスク（0 or 1 想定）
                oc = ms.gba; // 色（GBA）
            }
        }

    // マスク外は素通し
    if (m < 0.5f)
    {
        PixelShaderOutput outp;
        outp.color = gTexture.Sample(gSampler, input.texcoord);
        return outp;
    }

    // ── ここから下は元のエッジ検出ロジックをそのまま ──
    float2 uvStepSize = float2(1.0 / (float) width, 1.0 / (float) height);

    float2 difference = float2(0.0f, 0.0f);
    [unroll]
    for (int x = 0; x < 3; ++x)
    [unroll]
        for (int y = 0; y < 3; ++y)
        {
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gOutLineData.projectionInverse);
            float viewZ = viewSpace.z * rcp(viewSpace.w);
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];
        }

    float weight = length(difference);
    // くっきりさを上げたいなら：weight = step(0.4f, weight);
    weight = step(0.3f, weight);

    float3 baseCol = gTexture.Sample(gSampler, input.texcoord).rgb;

    PixelShaderOutput output;
    // 黒固定ではなく、マスクから拾った色 oc で縁取り
    output.color.rgb = lerp(baseCol, oc, weight);
    output.color.a = 1.0f;
    return output;
}

