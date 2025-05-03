#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
sampler gSampler : register(s0);

struct PixelShaderOutpot
{
    float4 color : SV_TARGET0;
};

PixelShaderOutpot main(VertexShaderOutput input)
{
    PixelShaderOutpot output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    float vignette = correct.x * correct.y * 16.0f;
    vignette = saturate(pow(vignette, 0.8f));
    output.color.rgb *= vignette;
    
    return output;
}