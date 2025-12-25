#pragma once

#include "../viewmotion.h"

class FallingMotion : public ViewMotion
{
public:

	FallingMotion();

	void update(float deltaTime, InputHandler& inputHandler, const Player& player) override;
private:
	float targetY;
};
