#include "Transform.h"
#include "../GameObject.h"
void Transform::UpdateWorldMatrix()
{
    XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    world = XMMatrixScaling(scale.x, scale.y, scale.z) * rotMat * XMMatrixTranslation(pos.x, pos.y, pos.z); //Scale Rotation Translation ¼ø¼­
    forward = XMVector3TransformNormal(DEFAULT_FORWARD_VECTOR, rotMat);
    backward = -forward;
    right = XMVector3TransformNormal(DEFAULT_RIGHT_VECTOR, rotMat);
    left = -right;
    up = XMVector3TransformNormal(DEFAULT_UP_VECTOR, rotMat);
    down = -up;

    if(gameObject != nullptr) gameObject->Update();
}

Transform::Transform()
{
    scale = { 1,1,1 };
    pos = rot = { 0,0,0 };
    posVec = rotVector = XMVectorSet(0, 0, 0, 1);
    UpdateWorldMatrix();
}

void Transform::Update()
{
}

const XMMATRIX& Transform::GetWorld()
{
    return world;
}

const XMVECTOR& Transform::GetForward() const
{
    return forward;
}

const XMVECTOR& Transform::GetBackward() const
{
    return backward;
}

const XMVECTOR& Transform::GetRight() const
{
    return right;
}

const XMVECTOR& Transform::GetLeft() const
{
    return left;
}

const XMVECTOR& Transform::GetUp() const
{
    return up;
}

const XMVECTOR& Transform::GetDown() const
{
    return down;
}

const XMVECTOR& Transform::GetPosVector() const
{
    return posVec;
}

const XMFLOAT3& Transform::GetPos() const
{
    return pos;
}

const XMVECTOR& Transform::GetRotVector() const
{
    return rotVector;
}

const XMFLOAT3& Transform::GetRot() const
{
    return rot;
}

void Transform::SetPos(const XMVECTOR& posVec)
{
    this->posVec = posVec;
    XMStoreFloat3(&pos, posVec);
    UpdateWorldMatrix();
}

void Transform::SetPos(float x, float y, float z)
{
    pos = { x,y,z };
    posVec = XMLoadFloat3(&pos);
    UpdateWorldMatrix();
}

void Transform::SetPos(XMFLOAT3& pos)
{
    SetPos(pos.x, pos.y, pos.z);
}

void Transform::Translate(const XMVECTOR& posVec)
{
    this->posVec += posVec;
    XMStoreFloat3(&pos, this->posVec);
    UpdateWorldMatrix();
}

void Transform::Translate(float x, float y, float z)
{
    pos.x += x;
    pos.y += y;
    pos.z += z;
    posVec = XMLoadFloat3(&pos);
    UpdateWorldMatrix();
}

void Transform::Translate(XMFLOAT3& pos)
{
    Translate(pos.x, pos.y, pos.z);
}

void Transform::SetRot(const XMVECTOR& rotVec)
{
    this->rotVector = rotVec;
    XMStoreFloat3(&rot, rotVec);
    UpdateWorldMatrix();
}

void Transform::SetRot(float x, float y, float z)
{
    rot = { x,y,z };
    rotVector = XMLoadFloat3(&rot);
    UpdateWorldMatrix();
}

void Transform::SetRot(XMFLOAT3& rot)
{
    SetRot(rot.x, rot.y, rot.z);
}

void Transform::Rotate(const XMVECTOR& rotVec)
{
    this->rotVector += rotVec;
    XMStoreFloat3(&rot, this->rotVector);
    UpdateWorldMatrix();
}

void Transform::Rotate(float x, float y, float z)
{
    rot.x += x;
    rot.y += y;
    rot.z += z;
    rotVector = XMLoadFloat3(&rot);
    UpdateWorldMatrix();
}

void Transform::Rotate(XMFLOAT3& rot)
{
    Rotate(rot.x, rot.y, rot.z);
}

void Transform::SetScale(float x, float y, float z)
{
    scale = { x,y,z };
    UpdateWorldMatrix();
}

void Transform::SetScale(XMFLOAT3& scale)
{
    SetScale(scale.x, scale.y, scale.z);
}

void Transform::LookAtPos(XMFLOAT3 target)
{
    LookAtPos(target.x, target.y, target.z);
}

void Transform::LookAtPos(float x, float y, float z)
{
    float yaw = 0.0f, pitch = 0.0f;
    double dx = (double)pos.x - x;
    double dy = (double)pos.y - y;
    double dz = (double)pos.z - z;
    double d = sqrt(dx * dx + dz * dz);
    //yaw
    if (dz != 0.0f)
    {
        yaw = atan(dx / dz);
        if (dz > 0.0f)
        {
            yaw += XM_PI;
        }
    }
    //pitch
    if (d != 0.0f)
    {
        pitch = atan(dy / d);
    }
    SetRot(pitch, yaw, 0.0f);
}
