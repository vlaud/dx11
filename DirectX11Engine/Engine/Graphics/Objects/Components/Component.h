#pragma once
class GameObject;
class Component
{
public:
	GameObject* gameObject = nullptr;
	virtual void Update() = 0;
};

