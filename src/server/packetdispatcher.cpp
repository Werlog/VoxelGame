#include "packetdispatcher.h"

PacketDispatcher::PacketDispatcher()
{

}

void PacketDispatcher::subscribe(unsigned short packetId, std::function<void(Packet&, unsigned short)> handleFunc)
{
	if (packetHandlers.find(packetId) != packetHandlers.end())
		return;
	packetHandlers.insert({ packetId, handleFunc });
}

void PacketDispatcher::dispatch(Packet& packet, unsigned short fromClientId)
{
	auto it = packetHandlers.find(packet.getPacketId());
	if (it == packetHandlers.end())
		return;

	it->second(packet, fromClientId);
}
