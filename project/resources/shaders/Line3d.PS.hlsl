struct Material
{
    float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PS_Output
{
    float4 color : SV_TARGET0;
};

PS_Output main()
{
    PS_Output output;
    output.color = gMaterial.color;
    return output; // 頂点から受け取った色をそのまま出力
}