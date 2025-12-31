#include "saving/worldsaver.h"
#include "GameState/PlayingGameState.h"
#include "world.h"
#include "saving/filewriter.h"
#include "saving/filereader.h"
#include "saving/filedatatypes.h"
#include "saving/worldsaveutil.h"
#include <filesystem>

WorldSaver::WorldSaver(World* world, PlayingGameState* playingState)
{
	this->world = world;
	this->playingState = playingState;
}

void WorldSaver::saveWorld()
{
	savePath = "saves/" + world->getWorldName();

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
	Player& player = playingState->getPlayer();
	glm::vec3 playerPos = player.getWorldPosition();
	BlockType* hotbarBlocks = player.getHotbarItems();

	SavedWorldInfo info = SavedWorldInfo{ world->getWorldSeed(), playerPos.x, playerPos.y, playerPos.z, player.getYaw(), player.getPitch() };

	for (size_t i = 0; i < 9; i++)
	{
		info.playerHotbar[i] = hotbarBlocks[i];
	}

	FileWriter writer = FileWriter(savePath + "/world.bin");
	writer.writeRaw(info);
}

void WorldSaver::ensureChunksDirectory()
{
	std::string path = std::string(RESOURCES_PATH + savePath + "/regions");
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

	world->getChunkManager().clearSavedChunks();
}

void WorldSaver::saveChunk(std::shared_ptr<Chunk> chunk)
{
	WorldRegion region = WorldRegion::toWorldRegion(chunk->getCoord());
	std::string regionPath = saveutil::getRegionPath(region, savePath);

	if (!std::filesystem::exists(regionPath))
	{
		createEmptyRegionFile(regionPath);
	}

	glm::ivec3 relPos = region.getRelativeChunkPosition(chunk->getCoord());
	int32_t lookupOffset = relPos.x + relPos.y * WORLD_REGION_SIZE + relPos.z * WORLD_REGION_SIZE * WORLD_REGION_SIZE;
	if (lookupOffset < 0)
	{
		return;
	}

	FileReader reader = FileReader(regionPath, false);
	reader.setStreamPosition(sizeof(RegionFileHeader) + lookupOffset * sizeof(int32_t));
	
	int32_t chunkDataOffset = 0;
	reader.readRaw(chunkDataOffset);

	if (chunkDataOffset == 0)
	{
		chunkDataOffset = reader.getFileSize();
	}

	reader.close();

	FileWriter writer = FileWriter(regionPath, false);
	writer.setStreamPosition(sizeof(RegionFileHeader) + lookupOffset * sizeof(int32_t));
	writer.writeRaw(chunkDataOffset);

	writer.setStreamPosition(chunkDataOffset);
	writer.writeRaw(chunk->getChunkStatus());
	writer.writeData((const char*)(chunk->getChunkData()), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * sizeof(BlockType));
	writer.close();
}

void WorldSaver::createEmptyRegionFile(const std::string& filePath)
{
	FileWriter writer = FileWriter(filePath, false);

	RegionFileHeader header = RegionFileHeader{
		"VOXEL_REGION",
		currentRegionVersion
	};
	writer.writeRaw(header);

	// Empty chunk lookup table
	writer.writeZeros(WORLD_REGION_SIZE * WORLD_REGION_SIZE * WORLD_REGION_SIZE * sizeof(int32_t));
}
