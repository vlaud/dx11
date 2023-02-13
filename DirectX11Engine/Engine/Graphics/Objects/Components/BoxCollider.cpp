#include "BoxCollider.h"
#include "../GameObject.h"
#include "Transform.h"

void BoxCollider::Initialize(float w, float h, float d)
{
	obb.axisDist[ASIX_X] = w / 2.0f;
	obb.axisDist[ASIX_Y] = h / 2.0f;
	obb.axisDist[ASIX_Z] = d / 2.0f;
	obb.axis[ASIX_X] = { 1,0,0 };
	obb.axis[ASIX_Y] = { 0,1,0 };
	obb.axis[ASIX_Z] = { 0,0,1 };
	obb.centerPos = { 0,0,0 };
}

const OBB BoxCollider::GetTransBB(XMMATRIX mat) const
{
	OBB temp;
	//XMVector3TransformCoord; //좌표를 변환
	//XMVector3TransformNormal;//방향 벡터를 변환
	XMVECTOR pos, dir, vecDist;
	pos = XMLoadFloat3(&obb.centerPos);
	pos = XMVector3TransformCoord(pos, mat);
	XMStoreFloat3(&temp.centerPos, pos);

	for (int i = 0; i < 3; ++i)
	{
		dir = XMLoadFloat3(&obb.axis[i]);
		dir = XMVector3TransformNormal(dir, mat);

		vecDist = XMVector3Length(dir);
		XMFLOAT3 dist;
		XMStoreFloat3(&dist, vecDist);
		temp.axisDist[i] = obb.axisDist[i] * dist.x;

		dir = XMVector3Normalize(dir);
		XMStoreFloat3(&temp.axis[i], dir);
	}
	return temp;
}

bool BoxCollider::CheckCrash(Collider& a, Collider& b)
{
	BoxCollider* box1 = dynamic_cast<BoxCollider*>(&a);
	BoxCollider* box2 = dynamic_cast<BoxCollider*>(&b);
	if (box1 == nullptr || box2 == nullptr) return false;

	OBB obb1, obb2;
	obb1 = box1->GetTransBB(box1->gameObject->GetTransform()->GetWorld());
	obb2 = box2->GetTransBB(box2->gameObject->GetTransform()->GetWorld());

	float c[3][3];
	float absC[3][3];
	float d[3];

	float dist, r1, r2;
	int i;

	const float cutoff = 0.99999f;
	bool existParallelPair = true;

	XMVECTOR centerDir = obb1.GetCenterPosVector() - obb2.GetCenterPosVector();

	XMFLOAT3 temp;
	for (int n = 0; n < 3; ++n)
	{
		for (i = 0; i < 3; ++i)
		{
			XMStoreFloat3(&temp, XMVector3Dot(obb1.GetAxisVector(n), obb2.GetAxisVector(i)));
			c[n][i] = temp.x;
			absC[n][i] = abs(c[n][i]);
			if (absC[n][i] > cutoff) existParallelPair = true;
		}
		XMStoreFloat3(&temp, XMVector3Dot(centerDir, obb1.GetAxisVector(n)));
		d[n] = temp.x;
		dist = abs(d[n]);
		r1 = obb1.axisDist[n];
		r2 = obb2.axisDist[ASIX_X] * absC[n][0] + obb2.axisDist[ASIX_Y] * absC[n][1] + obb2.axisDist[ASIX_Z] * absC[n][2];

		if (dist > r1 + r2) return false;
	}
	for (int n = 0; n < 3; ++n)
	{
		XMStoreFloat3(&temp, XMVector3Dot(centerDir, obb2.GetAxisVector(n)));
		dist = abs(temp.x);
		r2 = obb1.axisDist[ASIX_X] * absC[0][n] + obb1.axisDist[ASIX_Y] * absC[1][n] + obb1.axisDist[ASIX_Z] * absC[2][n];
		r1 = obb2.axisDist[n];
		if (dist > r1 + r2) return false;
	}
	if (existParallelPair) return true;

	for (int i = 0; i < 2; ++i)
	{
		dist = abs(d[2] * c[1][i] - d[1] * c[2][i]);
		r1 = obb1.axisDist[1] * absC[2][i] + obb1.axisDist[2] * absC[1][i];
		r2 = obb2.axisDist[i == 0 ? 1 : 0] * absC[0][i == 2 ? 1 : 2] + obb2.axisDist[i == 2 ? 1 : 2] * absC[0][i == 0 ? 1 : 0];
		if (dist > r1 + r2) return false;
	}
	for (int i = 0; i < 2; ++i)
	{
		dist = abs(d[1] * c[0][i] - d[0] * c[1][i]);
		r1 = obb1.axisDist[0] * absC[1][i] + obb1.axisDist[1] * absC[0][i];
		r2 = obb2.axisDist[i == 0 ? 1 : 0] * absC[2][i == 2 ? 1 : 2] + obb2.axisDist[i == 2 ? 1 : 2] * absC[2][i == 0 ? 1 : 0];
		if (dist > r1 + r2) return false;
	}
	for (int i = 0; i < 2; ++i)
	{
		dist = abs(d[0] * c[2][i] - d[2] * c[0][i]);
		r1 = obb1.axisDist[0] * absC[2][i] + obb1.axisDist[2] * absC[0][i];
		r2 = obb2.axisDist[i == 0 ? 1 : 0] * absC[1][i == 2 ? 1 : 2] + obb2.axisDist[i == 2 ? 1 : 2] * absC[1][i == 0 ? 1 : 0];
		if (dist > r1 + r2) return false;
	}

	return true;
}

void BoxCollider::Update()
{
}
