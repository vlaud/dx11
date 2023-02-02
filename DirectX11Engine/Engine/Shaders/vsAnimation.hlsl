#pragma pack_matrix(row_major)
#define MAX_BONE 256

cbuffer WVP : register(b0)
{
    matrix world;
    matrix vp;
}
cbuffer BoneData : register(b1)
{
    matrix bones[MAX_BONE];
    int hasAnimation;
}
struct VS_INPUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 boneIDs : BONEIDS; // �ִ� 4���� ���� ������ �޵���
    float4 weights : WEIGHTS; // ���� ���� ���� �޴� ����ġ��
};
struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};
matrix Skined(float4 boneList, float4 weightList)
{
    matrix mat = 0;
    mat += mul(weightList.x, bones[(uint) boneList.x]);
    mat += mul(weightList.y, bones[(uint) boneList.y]);
    mat += mul(weightList.z, bones[(uint) boneList.z]);
    mat += mul(weightList.w, bones[(uint) boneList.w]);
    return mat;
}
PS_INPUT vs(VS_INPUT input)
{
    PS_INPUT output;
    matrix boneWorld = world;
    if(hasAnimation !=0)
    {
        boneWorld = mul(Skined(input.boneIDs, input.weights), world);
    }
    output.pos = mul(float4(input.pos, 1), mul(boneWorld, vp));
    output.color = input.color;
    output.uv = input.uv;
    output.normal = normalize(mul(float4(input.normal, 0), world)); // float4�� w: (0 = ����(v), 1 = ��ġ(p)) p - p = v ���⺤��
    return output;
}