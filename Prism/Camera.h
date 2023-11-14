#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "Collider.h"

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	glm::vec3 forward, up, right;
	Collider collider = Collider(position, glm::vec3(1.0f));
	float fieldOfView = 45.0f;
	bool isFlightCam = false;

	Camera();
	Camera(glm::vec3 pos);
	Camera(glm::vec3 pos, bool isFlying);
	Camera(glm::vec3 pos, glm::vec3 rot, float fov);

	void Move(float x, float y, float z);
	void Move(glm::vec3 translation);
	void MoveWorldPosition(glm::vec3 translation);
	void Rotate(float yaw, float pitch);
	glm::mat4 GetViewMatrix();
private:
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	void updateCameraVectors();
	glm::mat4 lookAt();
	glm::mat4 calculate_lookAt_matrix();
};

#endif