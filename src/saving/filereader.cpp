#include "saving/filereader.h"

FileReader::FileReader(const std::string& filePath, bool isResourceFile)
{
	if (isResourceFile)
		this->stream = std::ifstream(std::string(RESOURCES_PATH "/" + filePath), std::ios::binary);
	else
		this->stream = std::ifstream(filePath, std::ios::binary);
}

FileReader::~FileReader()
{
	close();
}

uint64_t FileReader::getStreamPosition()
{
	return stream.tellg();
}

void FileReader::setStreamPosition(uint64_t position)
{
	stream.seekg(position);
}

uint64_t FileReader::getFileSize()
{
	std::streampos current = stream.tellg();

	stream.seekg(0, std::ios::end);
	uint64_t size = stream.tellg();

	stream.seekg(current);

	return size;
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

void FileReader::close()
{
	if (stream.is_open())
		stream.close();
}
