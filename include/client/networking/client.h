#pragma once

#include <enet/enet.h>

class Client
{
public:

	Client(unsigned short clientId);

	unsigned short getId() const;
private:
	unsigned short id;
};
