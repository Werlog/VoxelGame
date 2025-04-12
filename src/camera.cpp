#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, float fov, float aspectRatio) 
{
	this->position = position;
	this->fov = fov;

	firstMouse = true;

	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mouseSensitivity = 0.15f;

	yaw = 0;
	pitch = 0;

	updateProjectionMatrix(aspectRatio);
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update(float deltaTime)
{
	int xrel = 0;
	int yrel = 0;
	SDL_GetRelativeMouseState(&xrel, &yrel);
	ProccessMouse(xrel, yrel, true);
}

void Camera::ProccessMouse(float mouseX, float mouseY, bool constrainPitch)
{
	if (firstMouse)
	{
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		firstMouse = false;
	}

	float xOffset = mouseX;
	float yOffset = -mouseY;

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		else if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::updateProjectionMatrix(float aspectRatio)
{
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}