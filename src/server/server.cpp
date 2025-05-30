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
}

void Server::startServer()
{

}
