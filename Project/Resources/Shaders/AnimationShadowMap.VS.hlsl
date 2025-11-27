#include "object3d.hlsli"

struct InstanceData
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};
struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};
struct JointCount
{
    uint jointCount;
};
struct LightData
{
    float4x4 LightVP;
};
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

StructuredBuffer<InstanceData> gInstanceData : register(t0);
StructuredBuffer<Well> gMatrixPalette : register(t1);
StructuredBuffer<Material> gMaterial : register(t2);
ConstantBuffer<JointCount> gJointCount : register(b0);
ConstantBuffer<LightData> gLightData : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};
struct Skinned
{
    float4 position;
    float3 normal;
};

struct VSOut
{
    float4 position : SV_POSITION;
};

Skinned Skinning(VertexShaderInput input, uint id)
{
    Skinned skinned;
    
    int index = (gJointCount.jointCount * id);
    
    skinned.position = mul(input.position, gMatrixPalette[input.index.x + index].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y + index].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z + index].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w + index].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f;
    
    skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[input.index.x + index].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.y + index].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.z + index].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.w + index].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal);
    
    return skinned;
}

VSOut main(VertexShaderInput input, uint InstID : SV_InstanceID)
{
    VSOut output;
    
    if (!gMaterial[InstID].enableDraw || !gMaterial[InstID].shadowMap)
    {
        output.position = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }
    
    Skinned skinned = Skinning(input, InstID);

    float3 worldPos = mul(skinned.position, gInstanceData[InstID].World).xyz;
    output.position = mul(float4(worldPos, 1.0f), gLightData.LightVP);
    
    return output;
}