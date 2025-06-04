#include "server.h"
#include <iostream>
#include "logger.h"

Server::Server() : gameManager(this)
{
	host = nullptr;
	currentClientId = 0;
}

Server::~Server()
{
	if (host != nullptr)
		enet_host_destroy(host);
}

bool Server::init()
{
	if (enet_initialize() != 0)
	{
		logger::log("Failed to initialize ENet!", logger::ERR);
		return false;
	}
	atexit(enet_deinitialize);

	address.host = ENET_HOST_ANY;
	address.port = 2589;

	host = enet_host_create(&address, 20, 2, 0, 0);
	if (!host)
	{
		logger::log("Failed to create server host!", logger::ERR);
		return false;
	}

	logger::log("Server started on port 2589");
	return true;
}

void Server::sendPacket(Packet& packet, unsigned short toClient, bool reliable)
{
	auto it = clients.find(toClient);
	if (it == clients.end())
		return;

	ENetPacket* enetPacket = enet_packet_create(packet.getData(), packet.getLength(), reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);
	enet_peer_send(it->second.getPeer(), 0, enetPacket);
}

void Server::sendToAll(Packet& packet, bool reliable)
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		ENetPacket* enetPacket = enet_packet_create(packet.getData(), packet.getLength(), reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);
		enet_peer_send(it->second.getPeer(), 0, enetPacket);
	}
}

Client* Server::getClientFromPeer(ENetPeer* peer)
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.getPeer() == peer)
		{
			return &it->second;
		}
	}
	return nullptr;
}

Client* Server::getClientById(unsigned short clientId)
{
	auto it = clients.find(clientId);
	if (it == clients.end())
		return nullptr;
	return &it->second;
}

PacketDispatcher& Server::getDispatcher()
{
	return dispatcher;
}

void Server::startServer()
{
	logger::log("Waiting for connections...");

	while (true)
	{
		processEvents();
	}
}

void Server::processEvents()
{
	ENetEvent event;
	while (enet_host_service(host, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			char ip[16];

			if (enet_address_get_host_ip(&event.peer->address, ip, sizeof(ip)) != 0)
			{
				logger::log("Failed to get IP address of peer", logger::ERR);
				enet_peer_disconnect(event.peer, 0);
			}
			else
			{
				Client* newClient = addNewClient(event.peer);
				if (newClient != nullptr)
					logger::log(std::string(ip) + ":" + std::to_string(event.peer->address.port) + " successfully connected and is now client ID " + std::to_string(newClient->getId()));
				else
					enet_peer_disconnect(event.peer, 0);
			}
			break;
		case ENET_EVENT_TYPE_RECEIVE:
		{
			Client* client = getClientFromPeer(event.peer);

			if (client != nullptr)
			{
				try
				{
					Packet packet = Packet(reinterpret_cast<char*>(event.packet->data), event.packet->dataLength);
					dispatcher.dispatch(packet, client->getId());
				}
				catch (...)
				{
					logger::log("Failed to dispatch packet from client " + std::to_string(client->getId()) + ", disconnecting it.", logger::ERR);
					enet_peer_disconnect(event.peer, 0);
				}
			}
			else
			{
				logger::log("Received packet from peer without an assigned ID, disconnecting it.", logger::ERR);
				enet_peer_disconnect(event.peer, 0);
			}
		}
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			handleClientDisconnect(&event);
			break;
		}
	}
}

unsigned short Server::getNextClientId()
{
	currentClientId++;
	return currentClientId;
}

Client* Server::addNewClient(ENetPeer* peer)
{
	unsigned short id = getNextClientId();
	auto inserted = clients.try_emplace(id, peer, id);

	if (!inserted.second)
	{
		logger::log("Failed to add new client with id " + std::to_string(id) + "!", logger::ERR);
		return nullptr;
	}

	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		Packet packet = Packet(ServerToClient::S_ADD_CLIENT);
		packet.writeUShort(id);
		packet.writeByte(it->first == id ? 1 : 0); // Tells the client whether it is the newly added client
		sendPacket(packet, it->first, true);
	}

	return &inserted.first->second;
}

void Server::handleClientDisconnect(ENetEvent* event)
{
	Client* client = getClientFromPeer(event->peer);
	if (client == nullptr)
		return;

	Packet packet = Packet(ServerToClient::S_REMOVE_CLIENT);
	packet.writeUShort(client->getId());
	sendToAll(packet, true);

	logger::log("Client " + std::to_string(client->getId()) + " disconnected");

	clients.erase(client->getId());
	gameManager.handleClientDisconnect(client);
}

void Server::syncNewClient(Client* client)
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		Client& c = it->second;
		if (it->second.getId() == client->getId())
			continue;

		Packet packet = Packet(ServerToClient::S_ADD_CLIENT);
		packet.writeUShort(c.getId());
		packet.writeByte(0);
		sendPacket(packet, client->getId(), true);

		Player* player = gameManager.getPlayerById(c.getId());
		if (player != nullptr)
		{
			Packet playerPacket = Packet(ServerToClient::S_ADD_PLAYER);
			playerPacket.writeUShort(c.getId());
			playerPacket.writeString(player->getUsername());
			playerPacket.writeVec3(player->getPosition());
			sendPacket(playerPacket, client->getId(), true);
		}
	}
}
