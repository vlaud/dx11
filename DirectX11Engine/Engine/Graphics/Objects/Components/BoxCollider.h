#pragma once
#include "Collider.h"
#include <DirectXMath.h>
using namespace DirectX;

#define ASIX_X 0
#define ASIX_Y 1
#define ASIX_Z 2

struct OBB
{
	XMFLOAT3 centerPos;
	XMFLOAT3 axis[3];
	float axisDist[3];

	XMVECTOR GetCenterPosVector()
	{
		return XMLoadFloat3(&centerPos);
	}
	XMVECTOR GetAxisVector(int i)
	{
		return XMLoadFloat3(&axis[i]);
	}
};
class BoxCollider : public Collider
{
	OBB obb;
public:
	void Initialize(float w, float h, float d);
	const OBB GetTransBB(XMMATRIX mat) const;
	bool CheckCrash(Collider& a, Collider& b) override;
	void Update() override;
};

