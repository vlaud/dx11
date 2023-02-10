#pragma once
#include "Component.h"
class Collider : public Component
{
protected:
	bool isTrigger = false;
public:
	virtual bool CheckCrash(Collider& a, Collider& b) = 0;
	virtual void Update() override = 0;
};

