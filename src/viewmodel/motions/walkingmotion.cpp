#include "viewmodel/motions/walkingmotion.h"
#include <algorithm>
#include "util/math.h"

WalkingMotion::WalkingMotion()
{
	time = 0.0f;
	sinceMoved = 10.0f;
	sinceStopped = 0.0f;
}

void WalkingMotion::update(float deltaTime, InputHandler& inputHandler, const Player& player)
{
	time += deltaTime;

	if (isWalking(player, inputHandler))
	{
		sinceMoved = 0.0f;
		sinceStopped += deltaTime;

		weight = sinceStopped * 2.0f;
		weight = math::clamp(weight, 0.0f, 1.0f);
	}
	else
	{
		sinceMoved += deltaTime;
		sinceStopped = 0.0f;

		weight = 1.0f - sinceMoved * 4.0f;
		weight = math::clamp(weight, 0.0f, 1.0f);
	}

	offset = glm::vec3(cos(time * 7.0f) * 0.05f, sin(time * 14.0f) * 0.025f, 0.0f);
}

bool WalkingMotion::isWalking(const Player& player, InputHandler& inputHandler)
{
	bool walkingInput = inputHandler.getKey(SDLK_w) || inputHandler.getKey(SDLK_s)
		|| inputHandler.getKey(SDLK_a) || inputHandler.getKey(SDLK_d);

	return player.isPlayerGrounded() && glm::length(player.getVelocity()) > 1.0f && walkingInput;
}
