#pragma pack_matrix(row_major)

cbuffer buffer : register(b0)
{
    float4x4 world;
    float4x4 vp;
}

struct VS_IN
{
    float3 pos : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};
struct PS_IN
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};
PS_IN vs(VS_IN input)
{
    PS_IN output;

    output.pos = mul(float4(input.pos, 1), mul(world, vp));
    output.color = input.color;
    output.uv = input.uv;
    output.normal = normalize(mul(float4(input.normal, 0), world)); // float4의 w: (0 = 방향(v), 1 = 위치(p)) p - p = v 방향벡터
    output.tangent = normalize(mul(float4(input.tangent, 0), world));
    output.binormal = normalize(mul(float4(input.binormal, 0), world));

    return output;
}