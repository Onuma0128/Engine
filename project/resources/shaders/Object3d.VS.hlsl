#include "Object3d.hlsli"

struct InstanceData
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
    float4 color;
};
StructuredBuffer<InstanceData> gInstanceData : register(t0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint InstID : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gInstanceData[InstID].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gInstanceData[InstID].WorldInverseTranspose));
    output.worldPosition = mul(input.position, gInstanceData[InstID].World).xyz;
    return output;
}