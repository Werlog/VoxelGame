#include "gamemanager.h"
#include "server.h"
#include "logger.h"

GameManager::GameManager(Server* server)
{
	this->server = server;

	server->getDispatcher().subscribe(ClientToServer::C_LOGIN, [this](Packet& packet, unsigned short fromClientId) {
		this->handleLogin(packet, fromClientId);
	});
}

Player* GameManager::getPlayerById(unsigned short id)
{
	auto it = players.find(id);
	if (it == players.end())
		return nullptr;

	return &it->second;
}

void GameManager::handleLogin(Packet& packet, unsigned short fromClientId)
{
	std::string username = packet.readString();

	Client* client = server->getClientById(fromClientId);
	if (client == nullptr)
		return;

	auto inserted = players.try_emplace(fromClientId, client, username, glm::vec3(0.0f));

	Packet addPacket = Packet(ServerToClient::S_ADD_PLAYER);
	addPacket.writeUShort(fromClientId);
	addPacket.writeString(username);
	addPacket.writeVec3(glm::vec3(0.0f, 70.0f, 0.0f));
	server->sendToAll(addPacket, true);

	logger::log("Client " + std::to_string(fromClientId) + " logged in with the username \"" + username + "\"");

	server->syncNewClient(client);
}

void GameManager::handleClientDisconnect(Client* disconnectedClient)
{
	players.erase(disconnectedClient->getId());
}
