#include "viewmodel/motions/swingmotion.h"
#include "util/math.h"

SwingMotion::SwingMotion()
{
	this->rotation = glm::vec3(0.0f);
	this->animationProgress = 0.0f;
	this->isSwinging = false;
}

void SwingMotion::update(float deltaTime, InputHandler& inputHandler, const Player& player)
{
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_LEFT) || inputHandler.getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		swingArm();
	}
	if (!isSwinging)
		return;

	if (animationProgress < 0.5f)
	{
		offset = math::lerp(glm::vec3(0.0f), glm::vec3(0.5f, -0.3f, -0.35f), animationProgress * 2.0f);
		rotation = math::lerp(glm::vec3(0.0f), glm::vec3(0.0f, -30.0f, 60.0f), animationProgress * 2.0f);
	}
	else if (animationProgress > 0.5f)
	{
		offset = math::lerp(glm::vec3(0.5f, -0.3f, -0.35f), glm::vec3(0.0f), (animationProgress - 0.5f) * 2.0f);
		rotation = math::lerp(glm::vec3(0.0f, -30.0f, 60.0f), glm::vec3(0.0f), (animationProgress - 0.5f) * 2.0f);

		if (animationProgress >= 1.0f)
		{
			isSwinging = false;
		}
	}
	animationProgress += deltaTime / animationDuration;
}

void SwingMotion::swingArm()
{
	if (isSwinging)
	{
		if (animationProgress > 0.5f)
			animationProgress = 0.25f;
		return;
	}

	isSwinging = true;
	animationProgress = 0.0f;
}

glm::vec3 SwingMotion::getRotation() const
{
	return rotation;
}
