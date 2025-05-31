#include "server.h"
#include <iostream>
#include "logger.h"

Server::Server()
{
	host = nullptr;
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
			}
			else
			{
				logger::log("Incomming connection from " + std::string(ip) + ":" + std::to_string(event.peer->address.port));
			}
			break;
		}
	}
}
