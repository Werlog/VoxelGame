#include "gamemanager.h"
#include "server.h"

GameManager::GameManager(Server* server)
{
	this->server = server;

	server->getDispatcher().subscribe(ClientToServer::C_LOGIN, [this](Packet& packet, unsigned short fromClientId) {
		this->handleLogin(packet, fromClientId);
	});
}

void GameManager::handleLogin(Packet& packet, unsigned short fromClientId)
{
	
}
