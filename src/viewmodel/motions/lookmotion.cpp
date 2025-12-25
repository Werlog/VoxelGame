#include "viewmodel/motions/lookmotion.h"
#include "util/math.h"

LookMotion::LookMotion()
{
	targetOffset = glm::vec3(0.0f);
}

void LookMotion::update(float deltaTime, InputHandler& inputHandler, const Player& player)
{
	float mouseX = (float)inputHandler.getMouseX();
	float mouseY = (float)inputHandler.getMouseY();

	targetOffset = glm::vec3(-mouseX * 0.01f, mouseY * 0.01f, 0.0f);

	offset = math::lerp(offset, targetOffset, deltaTime * 5.0f);
}
