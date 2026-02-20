#include "UI/elements/inputfield.h"

InputField::InputField(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size)
	: UIElement(game, relPos, offset, size), minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf")),
		inputFieldTexture(game->getResourceManager().getTexture("textures\\UI\\inputField.png"))
{
	isSelected = false;
	showCursor = true;
	cursorTimer = 0.0f;
}

void InputField::update(InputHandler& inputHandler, float deltaTime)
{
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_LEFT))
	{
		bool shouldSelect = checkHovered(inputHandler);
		if (shouldSelect != isSelected)
		{
			if (shouldSelect)
			{
				inputHandler.startTextInput();
			}
			else
			{
				inputHandler.endTextInput();
				showCursor = true;
				cursorTimer = 0.0f;
			}
			isSelected = shouldSelect;
		}
	}

	if (isSelected)
	{
		if (!inputHandler.isReceivingTextInput())
			inputHandler.startTextInput();

		cursorTimer += deltaTime;
		if (cursorTimer >= 0.5f)
		{
			showCursor = !showCursor;
			cursorTimer = 0.0f;
		}

		if (inputHandler.getKey(SDLK_LCTRL))
		{
			pasteLogic(inputHandler);
			return;
		}

		if (inputHandler.getKeyDown(SDLK_BACKSPACE))
		{
			if (text.size() > 0)
			{
				this->text.pop_back();
				showCursor = true;
				cursorTimer = 0.0f;
				if (changedCallback) changedCallback(this->text);
			}
		}
		else
		{
			char* text = inputHandler.getTextInput();
			if (strlen(text) <= 0 || this->text.size() >= 24 || containsNonASCIIChars(text))
				return;
			this->text += std::string(text);
			if (changedCallback) changedCallback(this->text);
			showCursor = true;
			cursorTimer = 0.0f;
		}
	}
}

void InputField::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	uiRenderer->renderTexturedQuad(inputFieldTexture, position, scale, glm::vec2(1.0f));
	uiRenderer->renderText(minecraftFont, (isSelected && showCursor) ? text + "_" : text, position.x - scale.x + 10, position.y - (scale.y / 3), 0.7f, glm::vec3(1.0f));
}

void InputField::setOnChangedCallback(const std::function<void(const std::string&)>& func)
{
	this->changedCallback = func;
}

std::string InputField::getText()
{
	return text;
}

void InputField::pasteLogic(InputHandler& inputHandler)
{
	if (inputHandler.getKeyDown(SDLK_v))
	{
		char* clipboardText = SDL_GetClipboardText();
		if (containsNonASCIIChars(clipboardText))
			return;

		this->text += std::string(clipboardText);
	}
}

bool InputField::containsNonASCIIChars(char* str)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if ((unsigned char)str[i] > 127)
		{
			return true;
		}
	}
	return false;
}
