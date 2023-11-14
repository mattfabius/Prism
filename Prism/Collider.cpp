#include "Collider.h"

Collider::Collider()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(0.0f);
	bounds = Bounds();
}

Collider::Collider(glm::vec3 pos, glm::vec3 size)
{
	position = pos;
	scale = size;
	bounds = Bounds();
	bounds.min = pos - (size / 2.0f);
	bounds.max = pos + (size / 2.0f);
}

bool Collider::IsColliding(Collider &other)
{
	return bounds.isOverlappingBounds(other.bounds);
}

glm::vec3 Collider::GetOverlapAmount(Collider& other)
{
	float x1 = other.bounds.max.x - bounds.min.x;
	float x2 = bounds.max.x - other.bounds.min.x;
	float x = x1 < x2 ? -x1 : x2;
	
	float y1 = other.bounds.max.y - bounds.min.y;
	float y2 = bounds.max.y - other.bounds.min.y;
	float y = y1 < y2 ? -y1 : y2;

	float z1 = other.bounds.max.z - bounds.min.z;
	float z2 = bounds.max.z - other.bounds.min.z;
	float z = z1 < z2 ? -z1 : z2;

	glm::vec3 result(0.0f);
	if (std::abs(x) < std::abs(y) && std::abs(x) < std::abs(z))
	{
		result.x = x;
	}
	else if (std::abs(y) < std::abs(x) && std::abs(y) < std::abs(z))
	{
		result.y = y;
	}
	else 
	{
		result.z = z;
	}
	return result;
}
/*
	bool isOverlappingX(Bounds other) { return other.max.x >= min.x && max.x >= other.min.x; }
	bool isOverlappingY(Bounds other) { return other.max.y >= min.y && max.y >= other.min.y; }
	bool isOverlappingZ(Bounds other) { return other.max.z >= min.z && max.z >= other.min.z; }
*/
void Collider::SetPosition(glm::vec3 pos)
{
	position = pos;
	bounds.min = pos - (scale / 2.0f);
	bounds.max = pos + (scale / 2.0f);
}
