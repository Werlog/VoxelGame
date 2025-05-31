#pragma once
#include <enet/enet.h>

class Server
{
public:

	Server();

	~Server();

	bool init();

	void startServer();
private:
	ENetAddress address;
	ENetHost* host;

	void processEvents();
};
