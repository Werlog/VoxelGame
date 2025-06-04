#include "packet.h"
#include <stdexcept>
#include "endianutil.h"

Packet::Packet(unsigned short packetId)
{
	this->packetId = packetId;

	writeUShort(this->packetId);
}

Packet::Packet(char* data, size_t dataSize) : buffer(dataSize)
{
	std::memcpy(buffer.data(), data, dataSize);

	packetId = readUShort();
}

void Packet::writeInt(int value)
{
	value = endian::toBigEndian(value);
	writeBuffer(reinterpret_cast<const char*>(&value), sizeof(int));
}

void Packet::writeUShort(unsigned short value)
{
	value = endian::toBigEndian(value);
	writeBuffer(reinterpret_cast<const char*>(&value), sizeof(unsigned short));
}

void Packet::writeFloat(float value)
{
	if (endian::isLittleEndian())
		value = endian::swapFloatEndian(value);
	writeBuffer(reinterpret_cast<const char*>(&value), sizeof(float));
}

void Packet::writeByte(char value)
{
	writeBuffer(&value, sizeof(char));
}

void Packet::writeString(const std::string& string)
{
	writeInt(string.size());

	writeBuffer(string.c_str(), string.size());
}

void Packet::writeVec3(const glm::vec3& value)
{
	writeFloat(value.x);
	writeFloat(value.y);
	writeFloat(value.z);
}

int Packet::readInt()
{
	int value;
	readBuffer(reinterpret_cast<char*>(&value), sizeof(int));
	return endian::toHostEndian(value);
}

unsigned short Packet::readUShort()
{
	unsigned short value;
	readBuffer(reinterpret_cast<char*>(&value), sizeof(unsigned short));
	return endian::toHostEndian(value);
}

float Packet::readFloat()
{
	float value;
	readBuffer(reinterpret_cast<char*>(&value), sizeof(float));
	if (endian::isLittleEndian())
		value = endian::swapFloatEndian(value);
	return value;
}

char Packet::readByte()
{
	char value;
	readBuffer(&value, sizeof(char));
	return value;
}

std::string Packet::readString()
{
	int length = readInt();
	if (length < 0)
	{
		throw std::out_of_range("String length is negative");
	}
	std::vector<char> strBuffer(length);
	readBuffer(strBuffer.data(), length);
	return std::string(strBuffer.begin(), strBuffer.end());
}

glm::vec3 Packet::readVec3()
{
	float x = readFloat();
	float y = readFloat();
	float z = readFloat();

	return glm::vec3(x, y, z);
}

unsigned short Packet::getPacketId()
{
	return packetId;
}

const char* Packet::getData()
{
	return buffer.data();
}

size_t Packet::getLength()
{
	return buffer.size();
}

void Packet::writeBuffer(const char* data, size_t size)
{
	buffer.insert(buffer.end(), data, data + size);
}

void Packet::readBuffer(char* data, size_t size)
{
	if (readIndex + size > buffer.size())
	{
		throw std::out_of_range("Read exceeds buffer size");
	}
	std::memcpy(data, buffer.data() + readIndex, size);
	readIndex += size;
}
