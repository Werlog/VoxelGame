#pragma once

#include <enet/enet.h>
#include <string>
#include "packet.h"

constexpr float CONNECTION_TIMEOUT_TIME = 5.0f;

class ConnectionHandler
{
public:

	ConnectionHandler();

	bool init();
	bool isConnected() const;
	bool canConnect() const;

	void update(float deltaTime);

	void sendPacket(Packet& packet, bool reliable);

	void connect(const std::string& ipAddress, unsigned short port);
	void handleEvents();
private:
	ENetHost* client;
	ENetPeer* peer;

	bool connected;
	
	float sinceStartedConnecting;
};
