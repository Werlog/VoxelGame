#pragma once

#include "client.h"
#include <glm/glm.hpp>
#include <string>

class Player
{
public:

	Player(Client* client, const std::string& username, const glm::vec3 position);

	void setPosition(const glm::vec3& newPos);

	const std::string& getUsername() const;
	const glm::vec3& getPosition() const;
private:
	Client* client;
	std::string username;

	glm::vec3 position;
};