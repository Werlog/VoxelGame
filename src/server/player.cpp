#include "player.h"

Player::Player(Client* client, const std::string& username, const glm::vec3 position)
{
	this->client = client;
	this->username = username;
	this->position = position;
}

void Player::setPosition(const glm::vec3& newPos)
{
	position = newPos;
}

const std::string& Player::getUsername() const
{
	return username;
}

const glm::vec3& Player::getPosition() const
{
	return position;
}
