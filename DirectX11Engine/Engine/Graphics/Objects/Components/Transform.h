#pragma once
#include "Component.h"
#include <DirectXMath.h>
using namespace DirectX;

class Transform : public Component
{
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR posVec, rotVector, forward, backward, right, left, up, down;
	XMFLOAT3 pos, rot, scale;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_DOWN_VECTOR = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	void UpdateWorldMatrix();
public:
	Transform();
	void Update() override;
	const XMMATRIX& GetWorld();

	const XMVECTOR& GetForward() const;
	const XMVECTOR& GetBackward() const;
	const XMVECTOR& GetRight() const;
	const XMVECTOR& GetLeft() const;
	const XMVECTOR& GetUp() const;
	const XMVECTOR& GetDown() const;

	const XMVECTOR& GetPosVector() const;
	const XMFLOAT3& GetPos() const;
	const XMVECTOR& GetRotVector() const;
	const XMFLOAT3& GetRot() const;

	void SetPos(const XMVECTOR& posVec);
	void SetPos(float x, float y, float z);
	void SetPos(XMFLOAT3& pos);

	void Translate(const XMVECTOR& posVec);
	void Translate(float x, float y, float z);
	void Translate(XMFLOAT3& pos);

	void SetRot(const XMVECTOR& rotVec);
	void SetRot(float x, float y, float z);
	void SetRot(XMFLOAT3& rot);

	void Rotate(const XMVECTOR& rotVec);
	void Rotate(float x, float y, float z);
	void Rotate(XMFLOAT3& rot);

	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3& scale);

	void LookAtPos(XMFLOAT3 target);
	void LookAtPos(float x, float y, float z);
};

