#pragma pack_matrix(row_major) //�� �켱���� ��ȯ column_major : �� �켱

cbuffer buffer : register(b0)
{
    float4x4 wvp;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};
struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};
PS_INPUT vs(VS_INPUT input)
{
    PS_INPUT output;
    
    output.pos = mul(float4(input.pos, 1), wvp);
    output.uv = input.uv;
    return output;
}