#include <vector>
#include <string>

class Packet
{
public:

	Packet();

	void writeInt(int value);
	void writeUShort(unsigned short value);
	void writeFloat(float value);
	void writeByte(char value);
	void writeString(const std::string& string);

	int readInt();
	unsigned short readUShort();
	float readFloat();
	char readByte();
	std::string readString();
protected:
	std::vector<char> buffer;
	size_t readIndex = 0;

	void writeBuffer(const char* data, size_t size);
	void readBuffer(char* data, size_t size);
};
