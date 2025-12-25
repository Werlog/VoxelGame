#pragma once

#include "../viewmotion.h"

class EquipMotion : public ViewMotion
{
public:

	EquipMotion();

	void update(float deltaTime, InputHandler& inputHandler, const Player& player) override;

	void itemSwitched();
private:
	float sinceEquipped;
};
