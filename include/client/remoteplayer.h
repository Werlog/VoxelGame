#pragma once

#include <glm/glm.hpp>
#include "networking/client.h"
#include <string>
#include "game.h"
#include "chunkcoord.h"

class RemotePlayer
{
public:

	RemotePlayer(Client* client, Game* game, const std::string& username, const glm::vec3& position);

	void render(const ChunkCoord& playerCoord, const glm::vec3& cameraPos);
	void updatePosition(const glm::vec3 newPosition);

	const glm::vec3& getPosition() const;

	Client* getClient();
	unsigned short getId() const;
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::string username;
	glm::vec3 position;
	Client* client;
	Game* game;
};
