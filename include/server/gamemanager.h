#pragma once
#include <unordered_map>
#include "player.h"
#include "packet.h"

class Server;

class GameManager
{
public:

	GameManager(Server* server);

	void handleLogin(Packet& packet, unsigned short fromClientId);
private:
	std::unordered_map<unsigned short, Player> players;

	Server* server;
};
