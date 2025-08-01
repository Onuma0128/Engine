
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
};

struct VertexInfluence
{
    float4 weight;
    int4 index;
};

struct SkinningInformation
{
    uint numVertices;
    uint paletteStart;
    uint jointIndex;
};

struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};

struct NumVertices
{
    uint size;
    int pad[3];
};