#include "viewmodel/motions/equipmotion.h"

EquipMotion::EquipMotion()
{
	this->sinceEquipped = 0.0f;
}

void EquipMotion::update(float deltaTime, InputHandler& inputHandler, const Player& player)
{
	sinceEquipped += deltaTime;

	if (sinceEquipped > 0.25f)
	{
		offset = glm::vec3(0.0f);
		return;
	}

	offset = glm::vec3(0.0f, (sinceEquipped * 4.0f) - 1.0f, 0.0f);
}

void EquipMotion::itemSwitched()
{
	sinceEquipped = 0.0f;
}
