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

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDir : NORMAL1;
};
Texture2D tex : TEXTURE: register(t0);
SamplerState sampTex : SAMPLER: register(s0);

float4 ps(PS_INPUT input) : SV_Target
{
    float3 highlight = float3(0,0,0);
    float3 color = ambientColor * ambientStrength;
    if (lightStrength > 0.0f)
    {
        float3 dir = normalize(-lightDir);
        float lightIntensity = dot(input.normal, dir);
        if (lightIntensity > 0.0f)
        {
            color += lightColor * lightStrength * lightIntensity;    
        }     
        if (lightStrength > 0.9f)
        {
            float3 reflectionDir = normalize(lightDir + 2.0f * input.normal * dot(input.normal, -lightDir));
            highlight = saturate(pow(dot(reflectionDir, input.viewDir), lightStrength * 10.0f));
        }
    }
    float3 texColor = tex.Sample(sampTex, input.uv);
    color = saturate(color * texColor + highlight);

    return float4(color, alpha);
}