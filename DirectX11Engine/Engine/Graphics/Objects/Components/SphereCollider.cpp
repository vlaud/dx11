#include "SphereCollider.h"
#include "../GameObject.h"
#include "ComponentTypes.h"
#include <DirectXMath.h>
using namespace DirectX;

bool SphereCollider::CheckCrash(Collider& a, Collider& b)
{
    SphereCollider* sp1 = dynamic_cast<SphereCollider*>(&a);
    if (sp1 == nullptr) return false;
    SphereCollider* sp2 = dynamic_cast<SphereCollider*>(&b);
    if (sp2 == nullptr) return false;

    float r1, r2, distsq;
    XMFLOAT3 pos1, pos2;
    pos1 = sp1->gameObject->GetTransform()->GetPos();
    pos2 = sp2->gameObject->GetTransform()->GetPos();
    distsq = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2);
    r1 = sp1->GetRadius();
    r2 = sp2->GetRadius();

    if (pow((r1 + r2), 2) >= distsq)
    {
        return true;
    }
    return false;
}

void SphereCollider::Update()
{
}

void SphereCollider::Initialize(float radius)
{
    this->radius = radius;
}

const float& SphereCollider::GetRadius() const
{
    return radius;
}

