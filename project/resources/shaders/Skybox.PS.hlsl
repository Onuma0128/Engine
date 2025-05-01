#include "Skybox.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
    int xTexcoord_alpha;
    int yTexcoord_alpha;
};

ConstantBuffer<Material> gMaterial : register(b0);
TextureCube<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float3 dir = normalize(input.texcoord);
    float4 textureColor = gTexture.Sample(gSampler, dir);
    //if (gMaterial.xTexcoord_alpha != 0)
    //{
    //    textureColor.a *= input.texcoord.x;
    //}
    //if (gMaterial.yTexcoord_alpha != 0)
    //{
    //    textureColor.a *= input.texcoord.y;
    //}
    //textureColor.a = clamp(textureColor.a, 0.0f, 1.0f);
    
    output.color = textureColor * gMaterial.color;
    
    return output;
}