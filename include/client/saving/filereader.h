#pragma once

#include <cinttypes>
#include <string>
#include <fstream>

class FileReader
{
public:

	FileReader(const std::string& filePath, bool isResourceFile = true);
	~FileReader();

	uint64_t getStreamPosition();
	void setStreamPosition(uint64_t position);
	uint64_t getFileSize();

	bool readData(char* destination, size_t size);
	std::string readString();

	void close();

	template<typename T>
	void readRaw(T& type);
private:
	std::ifstream stream;
};

template<typename T>
inline void FileReader::readRaw(T& type)
{
	readData((char*)&type, sizeof(T));
}
