#include "remoteplayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

RemotePlayer::RemotePlayer(Client* client, Game* game, const std::string& username, const glm::vec3& position)
{
	this->client = client;
	this->game = game;
	this->username = username;
	this->position = position;
}

void RemotePlayer::render(const ChunkCoord& playerCoord, const glm::vec3& cameraPos)
{
	ChunkCoord coord = ChunkCoord::toChunkCoord(position);
	ChunkCoord relative = coord - playerCoord;
	glm::mat4 model = glm::mat4(1);
	glm::vec3 relPos = glm::vec3(relative.x * CHUNK_SIZE_X, relative.y * CHUNK_SIZE_Y, relative.z * CHUNK_SIZE_Z + 1);

	glm::vec3 start = relPos - glm::vec3(0.5f, 0.0f, 0.5f);
	glm::vec3 end = relPos + glm::vec3(0.5f, 2.0f, 0.5f);

	game->getDebugRenderer().renderBox(start, end, glm::vec3(1.0f, 0.0f, 0.0f));
}

void RemotePlayer::updatePosition(const glm::vec3 newPosition)
{
	this->position = position;
}

const glm::vec3& RemotePlayer::getPosition() const
{
	return position;
}

Client* RemotePlayer::getClient()
{
	return client;
}

unsigned short RemotePlayer::getId() const
{
	return client->getId();
}
