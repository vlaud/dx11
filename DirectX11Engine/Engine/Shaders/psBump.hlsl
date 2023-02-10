cbuffer Alpha : register(b0)
{
    float alpha;
}
cbuffer LightBuffer : register(b1) // constant buffer�� 16byte ������ ��� ó���ؾ���
{
    float3 ambientColor; // ���� ���� ���ϴ� ���� ��ο� ����
    float ambientStrength; // ��� Intensity
    float3 lightColor;
    float lightStrength;
    float3 lightDir;
}
struct PS_IN
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};
Texture2D tex : TEXTURE: register(t0);
Texture2D normalTex : TEXTURE: register(t1);
SamplerState sampTex : SAMPLER: register(s0);

float4 ps(PS_IN input) : SV_TARGET
{
    float3 texColor = tex.Sample(sampTex, input.uv);
    float3 color = ambientColor * ambientStrength;
    float3 bumpColor = normalTex.Sample(sampTex, input.uv);
    float3 bumpNormal = normalize(input.normal * bumpColor.z + input.tangent * bumpColor.x + input.binormal * bumpColor.y);
    //����� ���� �� ������ ����
    float3 dir = -normalize(lightDir);
    //���� ������ ���� ���� �󸶳� �޴��� ���
    float lightIntesity = dot(bumpNormal, dir);
    if (lightIntesity > 0)
    {
        color += lightColor * lightStrength * lightIntesity;
    }
    color = saturate(color * texColor);
    return float4(color, alpha);
}