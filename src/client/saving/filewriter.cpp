#include "saving/filewriter.h"
#include <filesystem>

FileWriter::FileWriter(const std::string& filePath, bool isResourceFile, bool eraseFile)
{
	std::string path = isResourceFile ? std::string(RESOURCES_PATH "/" + filePath) : filePath;

	if (eraseFile)
		stream = std::fstream(path, std::ios::binary | std::ios::out);
	else
	{
		stream = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);

		if (!this->stream)
		{
			stream.open(path, std::ios::out | std::ios::binary);
			stream.close();
			stream.open(path, std::ios::in | std::ios::out | std::ios::binary);
		}
	}
}

FileWriter::~FileWriter()
{
	close();
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

bool FileWriter::writeZeros(size_t size)
{
	char zero = 0;
	for (size_t i = 0; i < size; i++)
	{
		stream.write(&zero, sizeof(char));
	}
	return true;
}

void FileWriter::writeString(const std::string& value)
{
	writeRaw(value.size());

	writeData(value.data(), value.size() * sizeof(char));
}

void FileWriter::close()
{
	if (stream.is_open())
		stream.close();
}
