#pragma once

#include <enet/enet.h>

class Client
{
public:

	Client(ENetPeer* enetClient, unsigned short clientId);

	ENetPeer* getPeer();
	unsigned short getId() const;
private:
	ENetPeer* enetClient;
	unsigned short id;
};
