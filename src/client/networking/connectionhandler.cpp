#include "networking/connectionhandler.h"
#include <iostream>

ConnectionHandler::ConnectionHandler()
{
	client = nullptr;
	peer = nullptr;

	connected = false;
	sinceStartedConnecting = 0.0f;
}

bool ConnectionHandler::init()
{
	if (enet_initialize() != 0)
	{
		std::cout << "Failed to initialize ENet!" << std::endl;
		return false;
	}
	atexit(enet_deinitialize);

	client = enet_host_create(nullptr, 1, 2, 0, 0);
	if (!client)
	{
		std::cout << "Failed to create client host!" << std::endl;
		return false;
	}

	return true;
}

bool ConnectionHandler::isConnected() const
{
	return connected;
}

bool ConnectionHandler::canConnect() const
{
	return peer == nullptr;
}

void ConnectionHandler::update(float deltaTime)
{
	if (!canConnect() && !isConnected()) // Connecting
	{
		sinceStartedConnecting += deltaTime;
		if (sinceStartedConnecting >= CONNECTION_TIMEOUT_TIME)
		{
			std::cout << "Failed to connect to the server." << std::endl;
			enet_peer_reset(peer);
			peer = nullptr;
			sinceStartedConnecting = 0.0f;
		}
	}
}

void ConnectionHandler::sendPacket(Packet& packet, bool reliable)
{
	if (!isConnected()) return;

	ENetPacket* enetPacket = enet_packet_create(packet.getData(), packet.getLength(), reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);
	enet_peer_send(peer, 0, enetPacket);
	std::cout << "Packet sent" << std::endl;
}

void ConnectionHandler::connect(const std::string& ipAddress, unsigned short port)
{
	if (!canConnect()) return;

	ENetAddress targetAddress;
	enet_address_set_host(&targetAddress, ipAddress.c_str());
	targetAddress.port = port;

	peer = enet_host_connect(client, &targetAddress, 2, 0);
	if (!peer)
	{
		std::cout << "Unable to initiate the connection to " << ipAddress << ":" << std::to_string(port) << std::endl;
	}
	std::cout << "Connecting to " << ipAddress << ":" << std::to_string(port) << std::endl;
}

void ConnectionHandler::handleEvents()
{
	if (canConnect()) return;

	ENetEvent event;
	while (enet_host_service(client, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			std::cout << "Successfully connected to the server" << std::endl;
			connected = true;

			{
				Packet packet = Packet(ClientToServer::C_KEEP_ALIVE);
				sendPacket(packet, false);
			}
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << "Disconnected from the server" << std::endl;
			connected = false;
			enet_peer_reset(peer);
			peer = nullptr;
			break;
		}
	}
}
