#include "Object3d.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
    int enableDraw;
    int enableLighting;
    int outlineMask;
    float3 outlineColor;
    float shininess;
    float environmentCoefficient;
};

StructuredBuffer<Material> gMaterial : register(t0);

float4 main(VertexShaderOutput input) : SV_Target
{
    uint instID = input.instID;
    if (!gMaterial[instID].enableDraw || !gMaterial[instID].outlineMask)
    {   
        discard;
    }
    //return 1.0f;
    float3 outlineColor = gMaterial[instID].outlineColor;
    return float4(1.0f, outlineColor);
}