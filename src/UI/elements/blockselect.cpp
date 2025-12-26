#include "UI/elements/blockselect.h"

BlockSelect::BlockSelect(Game* game, glm::vec2 relPos, glm::vec2 offset, int rows, int columns, BlockIcons* blockIcons)
	: UIElement(game, relPos, offset, glm::vec2(columns * blockSelectCellSize, rows * blockSelectCellSize))
{
	this->game = game;
	this->rows = rows;
	this->columns = columns;
	this->blockIcons = blockIcons;

	this->hoveredBlock = -1;

	this->maxBlockId = blockIcons->getIconMap().size();
}

void BlockSelect::update(InputHandler& inputHandler, float deltaTime)
{
	bool isHovered = checkHovered(inputHandler);

	if (isHovered)
	{
		glm::vec2 position = getPosition();

		glm::vec2 mousePos = inputHandler.getMousePosition();
		int windowHeight = game->getUIRenderer().getWindowHeight();
		mousePos.y = windowHeight - mousePos.y;

		glm::vec2 topLeft = glm::vec2(position.x - blockSelectCellSize * columns, position.y + blockSelectCellSize * rows);
		glm::vec2 mouseRel = mousePos - topLeft;

		int row = (int)floor(-mouseRel.y / (blockSelectCellSize * 2));
		int column = (int)floor(mouseRel.x / (blockSelectCellSize * 2));

		hoveredBlock = column + row * columns + 1; // + 1 because air is not in the menu

		if (inputHandler.getMouseButtonUp(SDL_BUTTON_LEFT) && blockSelectFunc && hoveredBlock <= maxBlockId)
		{
			blockSelectFunc((BlockType)hoveredBlock);
		}
	}
	else
	{
		hoveredBlock = -1;
	}
}

void BlockSelect::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	uiRenderer->renderColoredQuad(glm::vec4(0.0f, 0.0f, 0.0f, 0.7f), position, scale);

	for (size_t i = 0; i < rows * columns; i++)
	{
		if (i == BlockType::AIR) continue;

		unsigned int texture = blockIcons->getTextureForBlock((BlockType)i);
		if (texture == 0) continue;

		int row = (i - 1) / columns;
		int column = (i - 1) - (columns * row);
		glm::vec2 pos = glm::vec2((-blockSelectCellSize * columns + blockSelectCellSize) + column * blockSelectCellSize * 2, (blockSelectCellSize * rows - blockSelectCellSize) - row * blockSelectCellSize * 2);

		if (i == hoveredBlock)
		{
			uiRenderer->renderColoredQuad(glm::vec4(0.85f), position + pos, glm::vec2(blockSelectCellSize));
		}
		uiRenderer->renderTexturedQuad(texture, position + pos, glm::vec2(blockSelectCellSize), glm::vec2(1.0f));
	}
}

void BlockSelect::setBlockSelectCallback(const std::function<void(BlockType)>& callback)
{
	this->blockSelectFunc = callback;
}
