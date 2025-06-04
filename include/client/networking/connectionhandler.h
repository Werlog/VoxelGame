#pragma once

#include <enet/enet.h>
#include <string>
#include <unordered_map>
#include "packet.h"
#include "packetdispatcher.h"
#include "client.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

class Game;

class ConnectionHandler
{
public:

	ConnectionHandler(Game* game);
	~ConnectionHandler();

	bool init();
	bool isConnected() const;
	bool canConnect() const;

	void update(float deltaTime);

	void sendPacket(Packet& packet, bool reliable);

	Client* getClientById(unsigned short clientId);
	Client* getLocalClient();

	PacketDispatcher& getDispatcher();

	void connect(const std::string& ipAddress, const std::string& username, unsigned short port);
	void handleEvents();
private:
	ENetHost* client;
	ENetPeer* serverPeer;
	PacketDispatcher dispatcher;

	Game* game;

	std::unordered_map<unsigned short, Client> clients;

	Client* localClient;

	std::string playerName;

	bool connected;
	
	float sinceStartedConnecting;

	void sendLogin();
	void onAddClient(Packet& packet);
	void onRemoveClient(Packet& packet);
	void disconnectCleanup();
};
