#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#define MAX_BONE 256
struct CB_VS_Offset
{
    XMFLOAT2 Offset;
};

struct CB_VS_Matrix
{
    XMMATRIX mat; //���� ���α׷���
};

struct CB_VS_WVP
{
    XMMATRIX world;
    XMMATRIX vp;
};
struct CB_PS_Alpha
{
    float alpha;
};

struct CB_PS_Light // constant buffer�� 16byte ������ ��� ó���ؾ���
{
    XMFLOAT3 ambientColor; // ���� ���� ���ϴ� ���� ��ο� ����
    float ambientStrength; // ��� Intensity
    XMFLOAT3 lightColor;
    float lightStrength;
    XMFLOAT3 lightDir;
    //float padding
};
struct CB_VS_BoneData
{
    XMMATRIX bones[MAX_BONE];
    int hasAnimation;
};