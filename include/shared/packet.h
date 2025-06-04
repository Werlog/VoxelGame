#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

enum ClientToServer : unsigned short
{
	C_KEEP_ALIVE,
	C_LOGIN,
};

enum ServerToClient : unsigned short
{
	S_KEEP_ALIVE,
	S_ADD_CLIENT,
	S_ADD_PLAYER,
	S_REMOVE_CLIENT,
};

class Packet
{
public:

	Packet(unsigned short packetId);
	Packet(char* data, size_t dataSize);

	void writeInt(int value);
	void writeUShort(unsigned short value);
	void writeFloat(float value);
	void writeByte(char value);
	void writeString(const std::string& string);
	void writeVec3(const glm::vec3& value);

	int readInt();
	unsigned short readUShort();
	float readFloat();
	char readByte();
	std::string readString();
	glm::vec3 readVec3();

	unsigned short getPacketId();

	const char* getData();
	size_t getLength();
protected:
	std::vector<char> buffer;
	size_t readIndex = 0;
	unsigned short packetId;

	void writeBuffer(const char* data, size_t size);
	void readBuffer(char* data, size_t size);
};
