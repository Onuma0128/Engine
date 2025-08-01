#include "object3d.hlsli"
#include "Skinning.hlsli"

struct InstanceData
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};

StructuredBuffer<InstanceData> gInstanceData : register(t0);
StructuredBuffer<Vertex> gVertexData : register(t1);
ConstantBuffer<NumVertices> gNumVertices : register(b0);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};

VertexShaderOutput main(VertexShaderInput input, uint vertexId : SV_VertexID, uint InstID : SV_InstanceID)
{
    VertexShaderOutput output;
    
    int index = InstID * gNumVertices.size + vertexId;
    Vertex data = gVertexData[index];
    
    output.position = mul(data.position, gInstanceData[InstID].WVP);
    output.worldPosition = mul(data.position, gInstanceData[InstID].World).xyz;
    output.texcoord = data.texcoord;
    output.normal = normalize(mul(data.normal, (float3x3) gInstanceData[InstID].WorldInverseTranspose));
    output.instID = InstID;
    return output;
}