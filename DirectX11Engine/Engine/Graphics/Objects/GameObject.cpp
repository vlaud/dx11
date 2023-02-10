#include "GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
GameObject::GameObject()
{
    shared_ptr<Transform> trans = make_shared<Transform>();
    AddComponent(trans);
}

Transform* GameObject::GetTransform()
{
    return dynamic_cast<Transform*>(components.begin()->get());
}

void GameObject::AddComponent(shared_ptr<Component> com)
{
    com->gameObject = this;
    components.push_back(com);
}

void GameObject::Update()
{
    for (shared_ptr<Component> com : components)
    {
        com->Update();
    }
}
