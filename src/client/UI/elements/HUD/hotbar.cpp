#include "UI/elements/HUD/hotbar.h"
#include "GameState/PlayingGameState.h"

Hotbar::Hotbar(Game* game, PlayingGameState* playingState, glm::vec2 relPos, glm::vec2 offset, BlockIcons* blockIcons)
	: UIElement(game, relPos, offset, glm::vec2(300.0f, 35.0f)),
	hotbarTexture(game->getResourceManager().getTexture("textures\\UI\\HUD\\hotbar.png")),
	selectedTexture(game->getResourceManager().getTexture("textures\\UI\\HUD\\hotbarSelected.png")),
	minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->blockIcons = blockIcons;
	this->selectedIndex = 0;
	this->sinceSwitched = 0.0f;
	this->playingState = playingState;

	for (size_t i = 0; i < hotbarBlocks.size(); i++)
	{
		hotbarBlocks[i] = (BlockType)(i + 1);
	}
}

void Hotbar::update(InputHandler& inputHandler, float deltaTime)
{
	int scroll = inputHandler.getMouseScroll();

	if (scroll != 0)
	{
		selectedIndex -= scroll;

		if (selectedIndex < 0)
		{
			selectedIndex = hotbarBlocks.size() - 1;
		}
		else if (selectedIndex >= hotbarBlocks.size())
		{
			selectedIndex = 0;
		}
		sinceSwitched = 0.0f;
	}
	sinceSwitched += deltaTime;
}

void Hotbar::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	if (sinceSwitched < 1.5f)
	{
		const BlockProperties& properties = playingState->getWorld().getBlockData().getBlockProperties(hotbarBlocks[selectedIndex]);
		uiRenderer->renderText(minecraftFont, properties.blockName, position.x, position.y + 75.0f, 0.75f, glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
	}

	uiRenderer->renderTexturedQuad(selectedTexture, position + glm::vec2(-scale.x + 37.0f + selectedIndex * 66.0f, 0.0f), glm::vec3(36.0f), glm::vec2(1.0f));
	for (int i = 0; i < hotbarBlocks.size(); i++)
	{
		BlockType type = hotbarBlocks[i];

		uiRenderer->renderTexturedQuad(blockIcons->getTextureForBlock(type), position + glm::vec2(-scale.x + 37.0f + i * 66.0f, 0.0f), glm::vec2(30.0f), glm::vec2(1.0f));
	}

	uiRenderer->renderTexturedQuad(hotbarTexture, position, scale, glm::vec2(1.0f));
}

BlockType Hotbar::getSelectedBlock() const
{
	return hotbarBlocks[selectedIndex];
}

void Hotbar::pickBlock(BlockType type)
{
	hotbarBlocks[selectedIndex] = type;
	sinceSwitched = 0.0f;
}
