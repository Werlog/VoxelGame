#pragma once

#include "../viewmotion.h"

class WalkingMotion : public ViewMotion
{
public:

	WalkingMotion();

	void update(float deltaTime, InputHandler& inputHandler, const Player& player) override;
private:
	bool isWalking(const Player& player, InputHandler& inputHandler);

	float sinceMoved;
	float sinceStopped;
	float time;
};
