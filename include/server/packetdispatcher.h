#pragma once

#include <unordered_map>
#include <functional>
#include "packet.h"

class PacketDispatcher
{
public:
	PacketDispatcher();

	void subscribe(unsigned short packetId, std::function<void(Packet&, unsigned short)> handleFunc);

	void dispatch(Packet& packet, unsigned short fromClientId);
private:
	std::unordered_map<unsigned short, std::function<void(Packet&, unsigned short)>> packetHandlers;
};
