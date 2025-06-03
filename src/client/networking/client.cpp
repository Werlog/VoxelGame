#include "networking/client.h"

Client::Client(unsigned short clientId)
{
	this->id = clientId;
}

unsigned short Client::getId() const
{
	return id;
}
