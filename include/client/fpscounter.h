#pragma once
#include "UI/uirenderer.h"

constexpr float frameUpdatePeriod = 0.25f;

class FpsCounter
{
public:

	FpsCounter();

	void init(ResourceManager& resourceManager);

	void update(float deltaTime);

	void render(UIRenderer& uiRenderer);
private:
	Font* minecraftFont;

	int frameCount;
	float sinceFrameUpdate;

	int frameRate;
};
