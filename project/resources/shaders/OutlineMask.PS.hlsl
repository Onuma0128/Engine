#include "Object3d.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
    int enableDraw;
    int enableLighting;
    int outlineMask;
    float shininess;
    float environmentCoefficient;
};

StructuredBuffer<Material> gMaterial : register(t0);

float main(VertexShaderOutput input) : SV_Target
{
    uint instID = input.instID;
    if (gMaterial[instID].enableDraw == 0 || gMaterial[instID].outlineMask == 0)
    {   
        discard;
    }
    return 1.0f;
}