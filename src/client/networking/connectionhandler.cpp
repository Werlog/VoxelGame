#include "networking/connectionhandler.h"
#include <iostream>

ConnectionHandler::ConnectionHandler()
{
	client = nullptr;
	serverPeer = nullptr;
	localClient = nullptr;

	connected = false;
	sinceStartedConnecting = 0.0f;

	dispatcher.subscribe(ServerToClient::S_ADD_CLIENT, [this](Packet& packet) {
		this->onAddClient(packet);
		});

	dispatcher.subscribe(ServerToClient::S_REMOVE_CLIENT, [this](Packet& packet) {
		this->onRemoveClient(packet);
		});

	dispatcher.subscribe(ServerToClient::S_ADD_PLAYER, [this](Packet& packet) {
		std::cout << "Received add player packet" << std::endl;
		});
}

ConnectionHandler::~ConnectionHandler()
{
	if (isConnected())
	{
		enet_peer_disconnect(serverPeer, 0);
	}

	enet_host_destroy(client);
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
	return serverPeer == nullptr;
}

void ConnectionHandler::update(float deltaTime)
{
	if (!canConnect() && !isConnected()) // Connecting
	{
		sinceStartedConnecting += deltaTime;
		if (sinceStartedConnecting >= CONNECTION_TIMEOUT_TIME)
		{
			std::cout << "Failed to connect to the server." << std::endl;
			enet_peer_reset(serverPeer);
			serverPeer = nullptr;
			sinceStartedConnecting = 0.0f;
		}
	}
}

void ConnectionHandler::sendPacket(Packet& packet, bool reliable)
{
	if (!isConnected()) return;

	ENetPacket* enetPacket = enet_packet_create(packet.getData(), packet.getLength(), reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);
	enet_peer_send(serverPeer, 0, enetPacket);
}

Client* ConnectionHandler::getClientById(unsigned short clientId)
{
	auto it = clients.find(clientId);

	if (it == clients.end())
		return nullptr;

	return &it->second;
}

void ConnectionHandler::connect(const std::string& ipAddress, const std::string& username, unsigned short port)
{
	if (!canConnect()) return;

	this->playerName = username;

	ENetAddress targetAddress;
	enet_address_set_host(&targetAddress, ipAddress.c_str());
	targetAddress.port = port;

	serverPeer = enet_host_connect(client, &targetAddress, 2, 0);
	if (!serverPeer)
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
			break;
		case ENET_EVENT_TYPE_RECEIVE:
		{
			Packet packet = Packet(reinterpret_cast<char*>(event.packet->data), event.packet->dataLength);
			dispatcher.dispatch(packet);
		}
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			disconnectCleanup();
			break;
		}
	}
}

void ConnectionHandler::sendLogin()
{
	Packet packet = Packet(ClientToServer::C_LOGIN);
	packet.writeString(playerName);
	sendPacket(packet, true);
}

void ConnectionHandler::onAddClient(Packet& packet)
{
	unsigned short clientId = packet.readUShort();
	bool isLocal = packet.readByte() == 1;

	auto inserted = clients.try_emplace(clientId, clientId);
	
	if (!inserted.second)
	{
		std::cout << "Failed to insert a new client!" << std::endl;
		return;
	}

	if (isLocal && localClient == nullptr)
	{
		localClient = &inserted.first->second;
		std::cout << "We are now client ID " << std::to_string(clientId) << std::endl;

		sendLogin();
	}
}

void ConnectionHandler::onRemoveClient(Packet& packet)
{
	unsigned short clientId = packet.readUShort();

	std::cout << "Client " << clientId << " disconnected." << std::endl;

	clients.erase(clientId);
}

void ConnectionHandler::disconnectCleanup()
{
	std::cout << "Disconnected from the server" << std::endl;
	connected = false;
	enet_peer_reset(serverPeer);
	serverPeer = nullptr;

	clients.clear();
}
