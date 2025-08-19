#include "Particle.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    int isUVFlipX;
    int isUVFlipY;
};
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float2 texcoord = input.texcoord;
    if (gMaterial.isUVFlipX != 0){ texcoord.x = 1.0f - texcoord.x; }
    if (gMaterial.isUVFlipY != 0){ texcoord.y = 1.0f - texcoord.y; }
    float4 transformedUV = mul(float4(texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    output.color = gMaterial.color * textureColor * input.color;
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}