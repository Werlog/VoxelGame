#pragma once

#include <glm/glm.hpp>
#include "player.h"

class ViewMotion
{
public:

	ViewMotion();

	virtual void update(float deltaTime, InputHandler& inputHandler, const Player& player) = 0;

	const glm::vec3& getOffset() const;
	float getWeight() const;
protected:
	glm::vec3 offset;
	float weight;
};
