#include "viewmodel/motions/fallingmotion.h"
#include "util/math.h"

FallingMotion::FallingMotion()
{
	targetY = 0.0f;
}

void FallingMotion::update(float deltaTime, InputHandler& inputHandler, const Player& player)
{
	targetY = player.getVelocity().y * (-0.01f);

	targetY = math::clamp(targetY, -0.5f, 0.5f);

	offset.y = math::lerp(offset.y, targetY, deltaTime * 10.0f);
}
