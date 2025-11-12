#include "Object3d.hlsli"

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

struct PSOut
{
    float4 mask : SV_Target0; // R=1(マスク), GBA=線色
    uint id : SV_Target1; // R32_UINT に書く
};

PSOut main(VertexShaderOutput input)
{
    uint instID = input.instID;
    if (!gMaterial[instID].enableDraw || !gMaterial[instID].outlineMask)
    {   
        PSOut o;
        o.mask = float4(0.25, 0.0, 0.0, 0.0);
        o.id = instID + 1u;
        return o;
    }
    if (!gMaterial[instID].outlineSceneColor)
    {
        PSOut o;
        o.mask = float4(0.5, gMaterial[instID].outlineColor);
        o.id = instID + 1u;
        return o;
    }

    PSOut output;
    output.mask = float4(1.0, gMaterial[instID].outlineColor);
    output.id = instID + 1u;
    return output;
}