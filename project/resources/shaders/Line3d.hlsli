struct VS_Output
{
    float4 position : SV_POSITION;
    nointerpolation uint instID : INSTANCEID0;
};