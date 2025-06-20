#pragma once

#include <glm/glm.hpp>
#include "inputhandler.h"

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

constexpr float nearPlane = 0.1f;
constexpr float farPlane = 1000.0f;

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float fov;

	float mouseSensitivity;

	Camera(glm::vec3 position, float fov, float aspectRatio);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void update(float deltaTime);
	void ProccessMouse(float mouseX, float mouseY, bool constrainPitch = true);
	void updateProjectionMatrix(float aspectRatio);

	void resetFirstMouse();
private:
	float lastMouseX, lastMouseY;
	bool firstMouse;
	glm::mat4 projectionMatrix;

	void updateCameraVectors();
};