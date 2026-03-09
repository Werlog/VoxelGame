#pragma once
#include "UI/uirenderer.h"

class FpsCounter
{
public:
	static constexpr float frameUpdatePeriod = 0.25f;

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
