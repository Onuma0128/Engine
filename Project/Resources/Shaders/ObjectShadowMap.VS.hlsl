#include "Object3d.hlsli"

struct InstanceData
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};
StructuredBuffer<InstanceData> gInstanceData : register(t0);

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
StructuredBuffer<Material> gMaterial : register(t1);

struct LightData
{
    float4x4 LightVP;
};
ConstantBuffer<LightData> gLightData : register(b0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VSOut
{
    float4 position : SV_POSITION;
};

VSOut main(VertexShaderInput input, uint InstID : SV_InstanceID)
{
    VSOut output;

    if (!gMaterial[InstID].enableDraw || !gMaterial[InstID].shadowMap)
    {
        output.position = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }
    
    float4 worldPos = mul(input.position, gInstanceData[InstID].World);

    output.position = mul(worldPos, gLightData.LightVP);
    
    return output;
}
