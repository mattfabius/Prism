#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>

class Bounds
{
public:
	glm::vec3 min;
	glm::vec3 max;
	Bounds() { min = glm::vec3(0.0f); max = glm::vec3(0.0f); }
	bool isOverlappingBounds(Bounds other) { return isOverlappingX(other) && isOverlappingY(other) && isOverlappingZ(other); }
private:
	bool isOverlappingX(Bounds other) { return other.max.x >= min.x && max.x >= other.min.x; }
	bool isOverlappingY(Bounds other) { return other.max.y >= min.y && max.y >= other.min.y; }
	bool isOverlappingZ(Bounds other) { return other.max.z >= min.z && max.z >= other.min.z; }
};

class Collider
{
public:
	Bounds bounds;
	Collider();
	Collider(glm::vec3 pos, glm::vec3 size);
	bool IsColliding(Collider &other); // AABB-AABB collision
	glm::vec3 GetOverlapAmount(Collider& other);
	void SetPosition(glm::vec3 pos);
private:
	glm::vec3 position;
	glm::vec3 scale;
};

#endif // !COLLIDER_H
