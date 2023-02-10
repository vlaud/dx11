#pragma once
#include <list>
#include <unordered_map>
#include <memory>
#include <string>
using namespace std;

class Component;
class Transform;

class GameObject
{
	list<shared_ptr<Component>> components;
	unordered_map<string, Component*> componentMap;
public:
	GameObject();
	Transform* GetTransform();
	void AddComponent(shared_ptr<Component> com);
	template<class T>
	T* GetComponent()
	{
		const char* name = typeid(T).name();
		if (componentMap.find(name) == componentMap.end())
		{
			T* ptr = nullptr;
			for (shared_ptr<Component> comp : components)
			{
				ptr = dynamic_cast<T*>(comp.get());
				if (ptr != nullptr)
				{
					componentMap[name] = comp.get();
					break;
				}
			}
			if(ptr == nullptr) return nullptr;
		}
		return dynamic_cast<T*>(componentMap[name]);
	}
	void Update();
};

