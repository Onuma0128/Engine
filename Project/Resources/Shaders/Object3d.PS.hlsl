#include "object3d.hlsli"

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
struct DirectionalLightData
{
    float4 color;
    float3 direction;
    float intensity;
};
struct PointLightData
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};
struct SpotLightData
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};
struct Camera
{
    float3 worldPosition;
};

struct KdColor
{
    float4 color;
};

ConstantBuffer<DirectionalLightData> gDirectionalLight : register(b1);
ConstantBuffer<PointLightData> gPointLight : register(b2);
ConstantBuffer<SpotLightData> gSpotLight : register(b3);
ConstantBuffer<Camera> gCamera : register(b4);
ConstantBuffer<KdColor> gKdColor : register(b5);

Texture2D<float4> gTexture : register(t0);
TextureCube<float4> gEnvironmentTexture : register(t1);
StructuredBuffer<Material> gMaterial : register(t2);
Texture2D<float> gShadowMap : register(t3);

SamplerState gSampler : register(s0);
SamplerComparisonState gShadowSampler : register(s1);
SamplerState gShadowSamplerNoCmp : register(s2);

/// =============================================================

static bool In01(float2 uv){ return uv.x>=0 && uv.x<=1 && uv.y>=0 && uv.y<=1; }
static float2 ShadowUV(float4 shadowPos){
    float2 ndc = shadowPos.xy / shadowPos.w;
    return float2(ndc.x * 0.5f + 0.5f,-ndc.y * 0.5f + 0.5f);
}
static float   ShadowZ (float4 shadowPos){ return saturate(shadowPos.z/shadowPos.w); } // LH 通常Z想定

/// =============================================================

float AdjustShadowWithNeighbors(float visibility, float2 shadowUV, float shadowZ, float bias)
{
    int width, height;
    gShadowMap.GetDimensions(width, height);
    float2 texel = float2(1.0 / width, 1.0 / height);

    float sum = 0.0f;
    const int radius = 3;

    [unroll]
    for (int y = -radius; y <= radius; ++y)
    {
        [unroll]
        for (int x = -radius; x <= radius; ++x)
        {
            float2 offset = float2(x, y) * texel;
            sum += gShadowMap.SampleCmpLevelZero(
                gShadowSampler,
                shadowUV + offset,
                shadowZ - bias
            );
        }
    }

    // 49 サンプルの平均値（0～1）
    float neighborAvg = sum / 49.0f;

    // visibility (中心1点) と の 差分調整はそのまま
    float diff = neighborAvg - visibility;
    float k = 0.5f;
    float result = saturate(visibility + diff * k);
    return lerp(0.2f, 1.0f, result);
}


float SampleShadow(VertexShaderOutput input, float3 normalWS, float3 lightDirWS)
{
    float3 N = normalize(normalWS);
    float3 L = normalize(lightDirWS);
    float ndotl = dot(N, L);

    // ライトの裏側はシャドウ計算しない
    if (ndotl <= 0.0f)
    {
        return 1.0f;
    }
    
    float2 uv = ShadowUV(input.shadowPosLS);
    float z = ShadowZ(input.shadowPosLS);
    if (!In01(uv))
        return 1.0f;

    ndotl = saturate(ndotl);

    // バイアス強め＆上向き面ほどバイアス大きめに
    float bias = 0.0001f;

    // これでもまだアクネが出るようなら bias をさらに上げる
    float shadow = gShadowMap.SampleCmpLevelZero(gShadowSampler, uv, z - bias);
    shadow = AdjustShadowWithNeighbors(shadow, uv, z, bias);
    
    return shadow;
}

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{    
    uint instID = input.instID;
    PixelShaderOutput output;
    // 描画を切っているならdiscardする
    if (!gMaterial[instID].enableDraw) { discard; }
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial[instID].uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 baseColor = textureColor.rgb * gKdColor.color.rgb;
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    // DirectionalLight
    float3 lightDirectionalLight = normalize(gDirectionalLight.direction);
    float3 reflectLight = reflect(-lightDirectionalLight, normal);
    float RdotE = max(dot(reflectLight, toEye), 0.0f);
    float specularPow = pow(RdotE, gMaterial[instID].shininess);
    // PointLight
    float3 pointLightDirecion = normalize(gPointLight.position - input.worldPosition);
    reflectLight = reflect(-pointLightDirecion, normal);
    RdotE = max(dot(reflectLight, toEye), 0.0f);
    float pointPow = pow(RdotE, gMaterial[instID].shininess);
    // SpotLight
    float3 spotLightDirecionOnSurface = normalize(gSpotLight.position - input.worldPosition);
    reflectLight = reflect(-spotLightDirecionOnSurface, normal);
    RdotE = max(dot(reflectLight, toEye), 0.0f);
    float spotPow = pow(RdotE, gMaterial[instID].shininess);
    
    if (gMaterial[instID].enableLighting != 0)
    {
        // DirectionalLightの処理
        float NdotL = (dot(normal, lightDirectionalLight));
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 diffuseDirectionalLight = gMaterial[instID].color.rgb * baseColor * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        // PointLightの処理
        NdotL = (dot(normal, pointLightDirecion));
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float distance = length(gPointLight.position - input.worldPosition);
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        float3 diffusePointLight = gMaterial[instID].color.rgb * baseColor * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        float3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity * pointPow * factor;
        // SpotLightの処理
        NdotL = (dot(normal, spotLightDirecionOnSurface));
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float cosAngle = dot(spotLightDirecionOnSurface, gSpotLight.direction);
        float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        float spotDistance = length(gSpotLight.position - input.worldPosition);
        float spotDistanceFactor = pow(saturate(-spotDistance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        float3 diffuseSpotLight = gMaterial[instID].color.rgb * baseColor * gSpotLight.color.rgb * cos * gSpotLight.intensity * spotDistanceFactor * falloffFactor;
        float3 specularSpotLight = gSpotLight.color.rgb * gSpotLight.intensity * spotPow * spotDistanceFactor * falloffFactor;
        // 環境マップの処理
        float3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
        float3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
        float4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
        environmentColor.rgb *= gMaterial[instID].environmentCoefficient;
        // ライトの処理を合算
        float visibility = SampleShadow(input, normal, lightDirectionalLight);
        
        // 各ライト成分はそのまま
        float3 dirLight = (diffuseDirectionalLight + specularDirectionalLight);
        float3 otherLights =
            (diffusePointLight + specularPointLight) +
            (diffuseSpotLight + specularSpotLight);

        // 太陽光だけ visibility を掛ける
        float3 litColor =
            environmentColor.rgb + // 環境光は常に有効
            otherLights +          // 他のライトも今回はシャドウ無し
            dirLight * visibility; // 平行光だけ影の影響を受ける

        output.color.rgb = litColor;
        output.color.a = gMaterial[instID].color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial[instID].color * float4(baseColor, textureColor.a);
    }

    if (textureColor.a == 0.0f || textureColor.a <= 0.5f || output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}