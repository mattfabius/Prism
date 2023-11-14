#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <list>

class GameObject
{
public:
	glm::vec3 localPosition = glm::vec3(0.0f);
	glm::vec3 localRotation = glm::vec3(0.0f);
	glm::vec3 localScale = glm::vec3(1.0f);
	
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();
	glm::vec3 GetWorldScale();

	void SetParent(GameObject* newParent);
	void AddChild(GameObject* newChild);
private:
	GameObject* parent = nullptr;
	std::list<GameObject*> children;
};

#endif // !GAMEOBJECT_H
