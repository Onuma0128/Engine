#include "Object3d.hlsli"

struct InstanceData
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};
StructuredBuffer<InstanceData> gInstanceData : register(t0);

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
    
    float4 worldPos = mul(input.position, gInstanceData[InstID].World);

    output.position = mul(worldPos, gLightData.LightVP);
    
    return output;
}
