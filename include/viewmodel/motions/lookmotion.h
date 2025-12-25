#pragma once

#include "../viewmotion.h"

class LookMotion : public ViewMotion
{
public:

	LookMotion();

	void update(float deltaTime, InputHandler& inputHandler, const Player& player) override;
private:
	glm::vec3 targetOffset;
};
