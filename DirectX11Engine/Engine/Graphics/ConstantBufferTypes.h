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
    XMMATRIX mat; //병렬 프로그래밍
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

struct CB_PS_Light // constant buffer는 16byte 단위로 끊어서 처리해야함
{
    XMFLOAT3 ambientColor; // 빛을 받지 못하는 가장 어두운 색깔
    float ambientStrength; // 밝기 Intensity
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