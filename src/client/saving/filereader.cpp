#include "saving/filereader.h"

FileReader::FileReader(const std::string& filePath)
{
	this->stream = std::ifstream(std::string(RESOURCES_PATH "\\" + filePath), std::ios::binary);
}

FileReader::~FileReader()
{
	stream.close();
}

uint64_t FileReader::getStreamPosition()
{
	return stream.tellg();
}

void FileReader::setStreamPosition(uint64_t position)
{
	stream.seekg(position);
}

bool FileReader::readData(char* destination, size_t size)
{
	stream.read(destination, size);
	return true;
}

std::string FileReader::readString()
{
	size_t size = 0;
	readData(reinterpret_cast<char*>(&size), sizeof(size_t));

	std::string str = std::string(' ', size);
	readData(str.data(), size);

	return str;
}
