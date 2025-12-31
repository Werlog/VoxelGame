#pragma once

#include "../viewmotion.h"

class SwingMotion : public ViewMotion
{
public:

	SwingMotion();

	void update(float deltaTime, InputHandler& inputHandler, const Player& player) override;

	void swingArm();

	glm::vec3 getRotation() const;
private:
	glm::vec3 rotation;

	static constexpr float animationDuration = 0.25f;

	float animationProgress;
	bool isSwinging;
};
