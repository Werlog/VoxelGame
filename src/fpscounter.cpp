#include "fpscounter.h"

FpsCounter::FpsCounter()
{
	frameCount = 0;
	frameRate = 0;
	sinceFrameUpdate = 0.0f;
	minecraftFont = nullptr;
}

void FpsCounter::init(ResourceManager& resourceManager)
{
	minecraftFont = &resourceManager.getFont("fonts\\MinecraftRegular.otf");
}

void FpsCounter::update(float deltaTime)
{
	sinceFrameUpdate += deltaTime;
	frameCount++;

	if (sinceFrameUpdate >= frameUpdatePeriod)
	{
		frameRate = frameCount / frameUpdatePeriod;
		frameCount = 0;
		sinceFrameUpdate -= frameUpdatePeriod;
	}
}

void FpsCounter::render(UIRenderer& uiRenderer)
{
	int width = uiRenderer.getWindowWidth();
	int height = uiRenderer.getWindowHeight();

	uiRenderer.renderText(*minecraftFont, "FPS: " + std::to_string(frameRate), 10, height - 50, 0.5f, glm::vec3(1.0f));
}
