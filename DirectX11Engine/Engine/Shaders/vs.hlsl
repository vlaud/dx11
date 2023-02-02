#pragma pack_matrix(row_major) //행 우선으로 변환 column_major : 열 우선

cbuffer buffer : register(b0)
{
    float4x4 world; //행렬은 더하기 연산 X, 곱하기만
    float4x4 vp;
}
struct VS_INPUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; //로컬 정보 Input Layout
};
struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};
PS_INPUT vs(VS_INPUT input)
{
    PS_INPUT output;
    
    output.pos = mul(float4(input.pos, 1), mul(world, vp));
    output.color = input.color;
    output.uv = input.uv;
    output.normal = normalize(mul(float4(input.normal, 0), world)); // float4의 w: (0 = 방향(v), 1 = 위치(p)) p - p = v 방향벡터
    return output;
}