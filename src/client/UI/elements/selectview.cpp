#include "UI/elements/selectview.h"
#include <iostream>

SelectView::SelectView(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::vector<std::string>& options)
	: UIElement(game, relPos, offset, size), minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->options = options;

	init();
}

SelectView::SelectView(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, std::initializer_list<const char*> options)
	: UIElement(game, relPos, offset, size), minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->options = std::vector<std::string>(options.begin(), options.end());

	init();
}

void SelectView::update(InputHandler& inputHandler, float deltaTime)
{
	int scroll = -inputHandler.getMouseScroll();
	bool hovered = checkHovered(inputHandler);

	if (scroll != 0 && hovered)
	{
		if (scroll < 0 && scrollIndex == 0)
			return;

		scrollIndex += scroll;

		int max = std::max(0, (int)options.size() - selectViewItemCount);
		if (scrollIndex > max)
		{
			scrollIndex = max;
		}
	}

	if (hovered && inputHandler.getMouseButtonUp(SDL_BUTTON_LEFT))
	{
		int clickedIndex = getHoveredIndex(inputHandler);

		if (clickedIndex < 0 || clickedIndex >= options.size())
		{
			selectedIndex = -1;
			return;
		}
		if (clickedIndex == selectedIndex)
		{
			selectedIndex = -1;
			return;
		}

		selectedIndex = clickedIndex;
	}
	else if (hovered)
	{
		hoveredIndex = getHoveredIndex(inputHandler);
	}
	else
	{
		hoveredIndex = -1;
	}
}

void SelectView::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	if (shouldShowScrollbar())
		renderScrollbar(uiRenderer);

	renderOptions(uiRenderer);

	uiRenderer->renderColoredQuad(glm::vec4(0.0f), position, scale);
}

int SelectView::getSelectedIndex() const
{
	return selectedIndex;
}

std::string SelectView::getSelectedOption() const
{
	if (selectedIndex < 0 || selectedIndex >= options.size())
		return "";

	return options[selectedIndex];
}

void SelectView::renderScrollbar(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	constexpr float scrollbarWidth = 12.0f;
	constexpr float scrollbarHeight = 20.0f;

	int scrollCount = options.size() - selectViewItemCount;
	float scrollStartPos = position.y + scale.y - scrollbarHeight;
	float scrollEndPos = position.y - scale.y + scrollbarHeight;
	float stepSize = (scrollStartPos - scrollEndPos) / scrollCount;

	uiRenderer->renderColoredQuad(glm::vec4(0.9f, 0.9f, 0.85f, 1.0f), glm::vec2(position.x + scale.x - scrollbarWidth, scrollStartPos - scrollIndex * stepSize), glm::vec2(scrollbarWidth, scrollbarHeight));
	uiRenderer->renderColoredQuad(glm::vec4(0.3f), glm::vec2(position.x + scale.x - scrollbarWidth, position.y), glm::vec2(scrollbarWidth, scale.y));
}

void SelectView::renderOptions(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();
	for (size_t i = scrollIndex; i < std::min(scrollIndex + selectViewItemCount, options.size()); i++)
	{
		bool selected = i == selectedIndex;

		float yPos = position.y + segmentHeight / 4 + scale.y - segmentHeight * (i - scrollIndex + 1);
		
		uiRenderer->renderText(minecraftFont, options[i], position.x, yPos, fontScale, selected ? glm::vec3(0.0f) : glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
		
		float selectionPos = position.y + segmentHeight / 2 + scale.y - segmentHeight * (i - scrollIndex + 1);
		if (selected)
		{
			uiRenderer->renderColoredQuad(glm::vec4(1.0f), glm::vec2(position.x, selectionPos), glm::vec2(scale.x, segmentHeight / 2.0f));
		}
		else if (hoveredIndex == i)
		{
			uiRenderer->renderColoredQuad(glm::vec4(0.6f), glm::vec2(position.x, selectionPos), glm::vec2(scale.x, segmentHeight / 2.0f));
		}
	}
}

int SelectView::getHoveredIndex(InputHandler& inputHandler)
{
	glm::vec2 position = getPosition();
	glm::vec2 mousePos = inputHandler.getMousePosition();
	mousePos.y = game->getUIRenderer().getWindowHeight() - mousePos.y;

	int segmentHovered = selectViewItemCount - floor((mousePos.y - position.y + scale.y) / segmentHeight) - 1;
	int hoveredIndex = scrollIndex + segmentHovered;

	return hoveredIndex;
}

void SelectView::init()
{
	scrollIndex = 0;

	selectedIndex = -1;
	hoveredIndex = -1;

	segmentHeight = (scale.y / selectViewItemCount) * 2.0f;
	float charHeight = minecraftFont.getCharacter('W').size.y;

	fontScale = std::min(segmentHeight / charHeight, selectViewMaxFontScale);
	std::cout << fontScale << std::endl;
}

bool SelectView::shouldShowScrollbar()
{
	return options.size() > selectViewItemCount;
}
