#include "client.h"

Client::Client(ENetPeer* enetClient, unsigned short clientId)
{
	this->enetClient = enetClient;
	this->id = clientId;
}

ENetPeer* Client::getPeer()
{
	return enetClient;
}

unsigned short Client::getId() const
{
	return id;
}
