#pragma once
#include "Collider.h"
class SphereCollider : public Collider
{
	float radius = 0.0f;
public:
	bool CheckCrash(Collider& a, Collider& b) override;
	void Update() override;
	void Initialize(float radius);
	const float& GetRadius() const;
};

