#pragma once

#include <cinttypes>
#include <string>
#include <fstream>

class FileReader
{
public:

	FileReader(const std::string& filePath);

	uint64_t getStreamPosition();
	void setStreamPosition(uint64_t position);

	bool readData(char* destination, size_t size);
	std::string readString();

	template<typename T>
	void readRaw(T& type);
private:
	std::ifstream stream;
};
