#include <iostream>
#include "server.h"

int main()
{
	Server server = Server();
	if (!server.init())
	{
		std::cout << "Unable to start the server." << std::endl;
		return 1;
	}
	server.startServer();
	return 0;
}
