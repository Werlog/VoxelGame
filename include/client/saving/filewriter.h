#pragma once

#include <fstream>
#include <string>
#include <cinttypes>

class FileWriter
{
public:

	FileWriter(const std::string& filePath);

	uint64_t getStreamPosition();
	void setStreamPosition(uint64_t position);

	bool writeData(const char* data, size_t size);
	void writeString(const std::string& value);

	template<typename T>
	void writeRaw(const T& type);
private:
	std::ofstream stream;
};
