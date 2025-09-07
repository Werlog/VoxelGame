#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, float fov, float aspectRatio) 
{
	this->position = position;
	this->fov = fov;

	firstMouse = true;

	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mouseSensitivity = 0.15f;

	aspectRatio = 1.0f;

	yaw = 0;
	pitch = 0;

	updateProjectionMatrix(aspectRatio);
	updateCameraVectors();
	updateFrustum();
}

const Frustum& Camera::getFrustum()
{
	return frustum;
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(glm::vec3 newPosition)
{
	position = newPosition;

	updateFrustum();
}

bool Camera::isInsideFrustum(AABB& aabb)
{
	return frustum.nearPlane.isOnOrForwardPlane(aabb) &&
		frustum.farPlane.isOnOrForwardPlane(aabb) &&
		frustum.leftPlane.isOnOrForwardPlane(aabb) &&
		frustum.rightPlane.isOnOrForwardPlane(aabb) &&
		frustum.topPlane.isOnOrForwardPlane(aabb) &&
		frustum.bottomPlane.isOnOrForwardPlane(aabb);
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
		return;
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
	updateFrustum();
}

void Camera::updateProjectionMatrix(float aspectRatio)
{
	aspectRatio = aspectRatio;
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

float Camera::getYaw() const
{
	return yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

void Camera::setYaw(float yaw)
{
	this->yaw = yaw;
	updateCameraVectors();
	updateFrustum();
}

void Camera::setPitch(float pitch)
{
	this->pitch = pitch;
	updateCameraVectors();
	updateFrustum();
}

void Camera::resetFirstMouse()
{
	firstMouse = true;
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

void Camera::updateFrustum()
{
	glm::mat4 viewProjectionMatrix = projectionMatrix * getViewMatrix();

	glm::vec4 row0 = glm::row(viewProjectionMatrix, 0);
	glm::vec4 row1 = glm::row(viewProjectionMatrix, 1);
	glm::vec4 row2 = glm::row(viewProjectionMatrix, 2);
	glm::vec4 row3 = glm::row(viewProjectionMatrix, 3);

	// Left
	frustum.leftPlane.normal.x = row3.x + row0.x;
	frustum.leftPlane.normal.y = row3.y + row0.y;
	frustum.leftPlane.normal.z = row3.z + row0.z;
	frustum.leftPlane.distance = row3.w + row0.w;

	// Right
	frustum.rightPlane.normal.x = row3.x - row0.x;
	frustum.rightPlane.normal.y = row3.y - row0.y;
	frustum.rightPlane.normal.z = row3.z - row0.z;
	frustum.rightPlane.distance = row3.w - row0.w;

	// Bottom
	frustum.bottomPlane.normal.x = row3.x + row1.x;
	frustum.bottomPlane.normal.y = row3.y + row1.y;
	frustum.bottomPlane.normal.z = row3.z + row1.z;
	frustum.bottomPlane.distance = row3.w + row1.w;

	// Top
	frustum.topPlane.normal.x = row3.x - row1.x;
	frustum.topPlane.normal.y = row3.y - row1.y;
	frustum.topPlane.normal.z = row3.z - row1.z;
	frustum.topPlane.distance = row3.w - row1.w;

	// Near
	frustum.nearPlane.normal.x = row3.x + row2.x;
	frustum.nearPlane.normal.y = row3.y + row2.y;
	frustum.nearPlane.normal.z = row3.z + row2.z;
	frustum.nearPlane.distance = row3.w + row2.w;

	// Far
	frustum.farPlane.normal.x = row3.x - row2.x;
	frustum.farPlane.normal.y = row3.y - row2.y;
	frustum.farPlane.normal.z = row3.z - row2.z;
	frustum.farPlane.distance = row3.w - row2.w;

	frustum.normalize();
}
