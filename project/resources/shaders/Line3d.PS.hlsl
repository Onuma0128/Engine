#include "Line3d.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
    int enableDraw;
    int enableLighting;
    int outlineMask;
    int outlineSceneColor;
    float3 outlineColor;
    float shininess;
    float environmentCoefficient;
    int shadowMap;
};

StructuredBuffer<Material> gMaterial : register(t0);

struct PS_Output
{
    float4 color : SV_TARGET0;
};

PS_Output main(VS_Output input)
{
    uint instID = input.instID;
    if (!gMaterial[instID].enableDraw)
    { 
        PS_Output o = { 0.0, 0.0, 0.0, 0.0 };
        return o;
    }
    
    PS_Output output;
    output.color = gMaterial[instID].color;
    return output;
}