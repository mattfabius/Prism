#include "Camera.h"

Camera::Camera()
{
	updateCameraVectors();
	//std::cout << "position: (" << position.x << "," << position.y << "," << position.z << ")\n" << "rotation: (" << rotation.x << "," << rotation.y << "," << rotation.z << ")\n" << "fov: " << fieldOfView << "\n" << std::endl;
}

Camera::Camera(glm::vec3 pos) 
{
	position = pos;
	collider.SetPosition(pos);
	updateCameraVectors();
}

Camera::Camera(glm::vec3 pos, bool isFlying)
{
	position = pos;
	collider.SetPosition(pos);
	isFlightCam = isFlying;
	updateCameraVectors();
}

Camera::Camera(glm::vec3 pos, glm::vec3 rot, float fov)
{
	position = pos;
	collider.SetPosition(pos);
	rotation = rot;
	fieldOfView = fov;
	updateCameraVectors();
}

void Camera::Move(float x, float y, float z)
{
	Move(glm::vec3(x, y, z));
}

void Camera::Move(glm::vec3 translation)
{
	position += right * translation.x;
	position += up * translation.y;
	glm::vec3 moveForward = forward;
	if (!isFlightCam)
		moveForward.y = 0.0f;
	position += moveForward * translation.z;
	collider.SetPosition(position);
}

void Camera::MoveWorldPosition(glm::vec3 translation)
{
	position += translation;
	collider.SetPosition(position);
}

void Camera::Rotate(float yaw, float pitch)
{
	rotation.x += pitch;
	rotation.y += yaw;

	rotation.x = clamp(rotation.x, -89.0f, 89.0f);
	rotation.y = glm::mod(rotation.y, 360.f);

	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = lookAt();
	return view;
}

void Camera::updateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	direction.y = sin(glm::radians(rotation.x));
	direction.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward = glm::normalize(direction);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

glm::mat4 Camera::lookAt()
{
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = right.x;
	rotation[1][0] = right.y;
	rotation[2][0] = right.z;
	rotation[0][1] = up.x;
	rotation[1][1] = up.y;
	rotation[2][1] = up.z;
	rotation[0][2] = -forward.x;
	rotation[1][2] = -forward.y;
	rotation[2][2] = -forward.z;
	
	return rotation * translation;
}
