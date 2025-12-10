#include "Sprite.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct DissolveParams
{
    float threshold;    // どこまで溶かすか
    float edgeWidth;    // 縁の幅
    float3 edgeColor;   // 縁の色
    float padding;
};
ConstantBuffer<DissolveParams> gDissolve : register(b1);

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gNoiseTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float2 uv = transformedUV.xy;

    float4 baseTextureColor = gTexture.Sample(gSampler, uv);
    float4 baseColor = gMaterial.color * baseTextureColor;

    float noise = gNoiseTexture.Sample(gSampler, uv);

    // 消す or 残す
    if (noise < gDissolve.threshold)
    {
        discard;
    }

    float edgeFactor = 1.0f - smoothstep(
        gDissolve.threshold,
        gDissolve.threshold + gDissolve.edgeWidth,
        noise
    );

    // 縁の色を加算（強すぎるなら 0.5 とかでスケール）
    float3 edgeGlow = gDissolve.edgeColor * edgeFactor;

    float4 finalColor = baseColor;
    finalColor.rgb += edgeGlow; // 本体の色は維持しつつ縁だけ光る

    output.color = finalColor;
    return output;
}

