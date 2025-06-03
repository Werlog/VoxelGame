#pragma once
#include <enet/enet.h>
#include <unordered_map>
#include "client.h"
#include "packet.h"
#include "packetdispatcher.h"

class Server
{
public:

	Server();

	~Server();

	bool init();

	void sendPacket(Packet& packet, unsigned short toClient, bool reliable);
	void sendToAll(Packet& packet, bool reliable);

	Client* getClientFromPeer(ENetPeer* peer);

	void startServer();
private:
	std::unordered_map<unsigned short, Client> clients;

	PacketDispatcher dispatcher;

	ENetAddress address;
	ENetHost* host;
	unsigned short currentClientId;

	void processEvents();
	unsigned short getNextClientId();
	Client* addNewClient(ENetPeer* peer);

	void onClientLogin(Packet& packet, unsigned short fromClientId);
};
