cbuffer Alpha : register(b0)
{
    float alpha;
}
cbuffer LightBuffer : register(b1) // constant buffer는 16byte 단위로 끊어서 처리해야함
{
    float3 ambientColor; // 빛을 받지 못하는 가장 어두운 색깔
    float ambientStrength; // 밝기 Intensity
    float3 lightColor;
    float lightStrength;
    float3 lightDir;
    //float padding
}
struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};
Texture2D tex : TEXTURE : register(t0);
SamplerState sampTex : SAMPLER : register(s0);

float4 ps(PS_INPUT input) : SV_Target
{
    float3 texColor = tex.Sample(sampTex, input.uv);
    float3 color = ambientColor * ambientStrength;
    //계산을 위해 빛 방향을 반전
    float3 dir = -normalize(lightDir);
    //내적 연산을 통해 빛을 얼마나 받는지 계산
    float lightIntesity = dot(input.normal, dir);
    if (lightIntesity > 0)
    {
        color += lightColor * lightStrength * lightIntesity;
    }
    color = saturate(color * texColor);
    return float4(color, alpha);
}