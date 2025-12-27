#include "viewmodel/motions/walkingmotion.h"
#include <algorithm>
#include "util/math.h"

WalkingMotion::WalkingMotion()
{
	time = 0.0f;
	movedWeight = 0.0f;
	stoppedWeight = 0.0f;
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

		weight = math::lerp(stoppedWeight, 1.0f, math::cubicEase(math::clamp(sinceStopped * 2.0f, 0.0f, 1.0f)));
		movedWeight = weight;
	}
	else
	{
		sinceMoved += deltaTime;
		sinceStopped = 0.0f;

		weight = math::lerp(movedWeight, 0.0f, math::cubicEase(math::clamp(sinceMoved * 2.0f, 0.0f, 1.0f)));
		stoppedWeight = weight;
	}

	offset = glm::vec3(cos(time * 7.0f) * 0.1f, sin(time * 14.0f) * 0.07f, 0.0f);
}

bool WalkingMotion::isWalking(const Player& player, InputHandler& inputHandler)
{
	bool walkingInput = inputHandler.getKey(SDLK_w) || inputHandler.getKey(SDLK_s)
		|| inputHandler.getKey(SDLK_a) || inputHandler.getKey(SDLK_d);

	return player.isPlayerGrounded() && glm::length(player.getVelocity()) > 3.0f && walkingInput;
}
