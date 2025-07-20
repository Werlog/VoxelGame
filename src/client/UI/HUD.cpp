#include "UI/HUD.h"

HUD::HUD(Game* game, PlayingGameState* playingState, BlockIcons* blockIcons)
	: hotbar(game, playingState, glm::vec2(0.5f, 0.0f), glm::vec2(0.0f, 35.0f), blockIcons)
{
	this->game = game;
}

void HUD::update(InputHandler& inputHandler, float deltaTime)
{
	hotbar.update(inputHandler, deltaTime);
}

void HUD::render(UIRenderer* uiRenderer)
{
	hotbar.render(uiRenderer);

	int width = uiRenderer->getWindowWidth();
	int height = uiRenderer->getWindowHeight();
	uiRenderer->renderCrosshair(width / 2.0f, height / 2.0f, 20.0f);
}

Hotbar& HUD::getHotbar()
{
	return hotbar;
}
