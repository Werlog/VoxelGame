#include "saving/filewriter.h"

FileWriter::FileWriter(const std::string& filePath)
{
	this->stream = std::ofstream(std::string(RESOURCES_PATH "\\" + filePath), std::ios::binary);
}

FileWriter::~FileWriter()
{
	stream.close();
}

uint64_t FileWriter::getStreamPosition()
{
	return stream.tellp();
}

void FileWriter::setStreamPosition(uint64_t position)
{
	stream.seekp(position);
}

bool FileWriter::writeData(const char* data, size_t size)
{
	stream.write(data, size);
	return true;
}

void FileWriter::writeString(const std::string& value)
{
	writeRaw(value.size());

	writeData(value.data(), value.size() * sizeof(char));
}