#pragma once

#include "generationFeature.h"

class OakTreeFeature : public GenerationFeature
{
public:

	OakTreeFeature(glm::vec3 origin, int height, bool isModification = false);

	void setOrigin(glm::vec3 origin);

	void generate(std::shared_ptr<Chunk> chunk) const override;
private:
	int height;
	bool isModification;
};
