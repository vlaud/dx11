#pragma pack_matrix(row_major) //�� �켱���� ��ȯ column_major : �� �켱

cbuffer buffer : register(b0)
{
    float4x4 world; //����� ���ϱ� ���� X, ���ϱ⸸
    float4x4 vp;
}

cbuffer CameraBuffer : register(b1)
{
    float3 CameraPos;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; //���� ���� Input Layout
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDir : NORMAL1;
};
PS_INPUT vs(VS_INPUT input)
{
    PS_INPUT output;

    output.pos = mul(float4(input.pos, 1), mul(world, vp));
    output.color = input.color;
    output.uv = input.uv;
    output.normal = normalize(mul(float4(input.normal, 0), world));
    float4 worldPos = mul(float4(input.pos, 1), world);
    output.viewDir = normalize(CameraPos.xyz - worldPos.xyz);

    return output;
}