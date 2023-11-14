#include "GameObject.h"

glm::vec3 GameObject::GetWorldPosition()
{
	if (parent == nullptr)
	{
		return localPosition;
	}
	return localPosition + parent->GetWorldPosition();
}

glm::vec3 GameObject::GetWorldRotation()
{
	if (parent == nullptr)
	{
		return localRotation;
	}
	return localRotation + parent->GetWorldRotation();
}

glm::vec3 GameObject::GetWorldScale()
{
	if (parent == nullptr)
	{
		return localScale;
	}
	return localScale * parent->GetWorldScale();
}

void GameObject::SetParent(GameObject* newParent)
{
	parent = newParent;
	parent->AddChild(this);
}

void GameObject::AddChild(GameObject* newChild)
{
	children.push_back(newChild);
}
