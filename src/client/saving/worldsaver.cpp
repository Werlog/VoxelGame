#include "saving/worldsaver.h"
#include "GameState/PlayingGameState.h"
#include "world.h"
#include "saving/filewriter.h"
#include "saving/filereader.h"
#include "saving/savedworldinfo.h"
#include <filesystem>
#include <sstream>

WorldSaver::WorldSaver(World* world, PlayingGameState* playingState)
{
	this->world = world;
	this->playingState = playingState;
}

void WorldSaver::saveWorld()
{
	savePath = "saves\\" + world->getWorldName();

	ensureDirectory();
	saveWorldInfoFile();

	ensureChunksDirectory();
	saveChunks();
}

void WorldSaver::ensureDirectory()
{
	std::string path = std::string(RESOURCES_PATH + savePath);
	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::create_directory(path);
	}
}

void WorldSaver::saveWorldInfoFile()
{
	glm::vec3 playerPos = playingState->getPlayer().getWorldPosition();

	SavedWorldInfo info = SavedWorldInfo{ world->getWorldSeed(), playerPos.x, playerPos.y, playerPos.z };

	FileWriter writer = FileWriter(savePath + "\\world.bin");
	writer.writeRaw(info);
}

void WorldSaver::ensureChunksDirectory()
{
	std::string path = std::string(RESOURCES_PATH + savePath + "\\regions");
	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::create_directory(path);
	}
}

void WorldSaver::saveChunks()
{
	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& loadedChunks = world->getChunkManager().getLoadedChunks();

	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		const std::shared_ptr<Chunk>& chunk = it->second;

		if (chunk->wasModified())
		{
			saveChunk(chunk);
		}
	}

	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& savedChunks = world->getChunkManager().getSavedChunks();

	for (auto it = savedChunks.begin(); it != savedChunks.end(); it++)
	{
		saveChunk(it->second);
	}
}

void WorldSaver::saveChunk(std::shared_ptr<Chunk> chunk)
{
	WorldRegion region = WorldRegion::toWorldRegion(chunk->getCoord());
	std::string regionPath = getRegionPath(region);

	if (!std::filesystem::exists(regionPath))
	{
		createEmptyRegionFile(regionPath);
	}

	glm::ivec3 chunkPos = region.getRelativeChunkPosition(chunk->getCoord());
	int32_t lookupOffset = chunkPos.x + chunkPos.y * WORLD_REGION_SIZE + chunkPos.z * WORLD_REGION_SIZE * WORLD_REGION_SIZE;
	if (lookupOffset < 0)
	{
		return;
	}

	FileReader reader = FileReader(regionPath, false);
	reader.setStreamPosition(lookupOffset * sizeof(int32_t));
	
	int32_t chunkDataOffset = 0;
	reader.readRaw(chunkDataOffset);

	if (chunkDataOffset == 0)
	{
		chunkDataOffset = reader.getFileSize();
	}

	reader.close();

	FileWriter writer = FileWriter(regionPath, false);
	writer.setStreamPosition(lookupOffset * sizeof(int32_t));
	writer.writeRaw(chunkDataOffset);

	writer.setStreamPosition(chunkDataOffset);
	writer.writeData((const char*)(chunk->getChunkData()), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * sizeof(BlockType));
	writer.close();
}

std::string WorldSaver::getRegionPath(WorldRegion region)
{
	std::ostringstream oss;
	oss << RESOURCES_PATH << savePath << "/regions/" << "r." << region.x << "." << region.y << "." << region.z << ".bin";

	return oss.str();
}

void WorldSaver::createEmptyRegionFile(const std::string& filePath)
{
	FileWriter writer = FileWriter(filePath, false);
	// Empty chunk lookup table
	writer.writeZeros(WORLD_REGION_SIZE * WORLD_REGION_SIZE * WORLD_REGION_SIZE * sizeof(int32_t));
}
