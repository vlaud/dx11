#pragma once
#include <DirectXMath.h>
#include "Component.h"
using namespace DirectX;

enum class CameraProjectionType
{
	Perspective, Ortho
};

class Camera : public Component
{
	XMMATRIX view, proj;
	void UpdateView();
public:
	CameraProjectionType type = CameraProjectionType::Perspective;
	Camera();
	void SetPerspective(float fov, float aspect, float nearZ, float farZ);
	void SetOrtho(float w, float h, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetprojectionMatrix() const;
	const XMMATRIX& GetViewprojection() const;
	void Update() override;
};

