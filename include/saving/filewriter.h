#pragma once

#include <fstream>
#include <string>
#include <cinttypes>

class FileWriter
{
public:

	FileWriter(const std::string& filePath, bool isResourceFile = true, bool eraseFile = false);
	~FileWriter();

	uint64_t getStreamPosition();
	void setStreamPosition(uint64_t position);

	bool writeData(const char* data, size_t size);
	bool writeZeros(size_t size);
	void writeString(const std::string& value);

	void close();

	template<typename T>
	void writeRaw(const T& type);
private:
	std::fstream stream;
};

template<typename T>
void FileWriter::writeRaw(const T& type)
{
	writeData((char*)&type, sizeof(T));
}
