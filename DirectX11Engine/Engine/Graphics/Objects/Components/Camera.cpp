#include "Camera.h"
#include "../GameObject.h"
void Camera::UpdateView()
{
	XMVECTOR camFocus = gameObject->GetTransform()->GetPosVector() + gameObject->GetTransform()->GetForward();
	view = XMMatrixLookAtLH(gameObject->GetTransform()->GetPosVector(), camFocus, gameObject->GetTransform()->GetUp());
}

Camera::Camera()
{
	proj = view = XMMatrixIdentity();
}

void Camera::SetOrtho(float w, float h, float nearZ, float farZ)
{
	type = CameraProjectionType::Ortho;
	proj = XMMatrixOrthographicOffCenterLH(0.0f, w, h, 0.0f, nearZ, farZ);
}

void Camera::SetPerspective(float fov, float aspect, float nearZ, float farZ)
{
	type = CameraProjectionType::Perspective;
	//투영 행렬
	float rad = (fov * XM_2PI) / 360.0f;
	proj = XMMatrixPerspectiveFovLH(rad, aspect, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return view;
}

const XMMATRIX& Camera::GetprojectionMatrix() const
{
	return proj;
}

const XMMATRIX& Camera::GetViewprojection() const
{
	return XMMatrixMultiply(view, proj);
}

void Camera::Update()
{
	UpdateView();
}