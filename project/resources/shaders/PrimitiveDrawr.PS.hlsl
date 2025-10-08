#include "PrimitiveDrawr.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
    int xTexcoord_alpha;
    int yTexcoord_alpha;
    int pad[2];
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float2 uv = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform).xy;
    float2 dudx = ddx(uv);
    float2 dudy = ddy(uv);
    float2 uvWrap = uv - floor(uv);
    
    uint w, h, levels;
    gTexture.GetDimensions(0, w, h, levels);
    float2 texSize = float2(w, h);
    float2 uvSafe = (uvWrap * (texSize - 1.0f) + 0.5f) / texSize;
    
    float4 textureColor = gTexture.SampleGrad(gSampler, uvSafe, dudx, dudy);
    if (gMaterial.xTexcoord_alpha != 0)
    {
        textureColor.a *= input.texcoord.x;
    }
    if (gMaterial.yTexcoord_alpha != 0)
    {
        textureColor.a *= input.texcoord.y;
    }
    textureColor.a = clamp(textureColor.a, 0.0f, 1.0f);
    
    output.color = gMaterial.color * textureColor;
    if (output.color.a <= 0.025f)
    {
        discard;
    }
    return output;
}