#include "saving/worldloader.h"
#include "saving/filereader.h"
#include "saving/worldsaveutil.h"
#include "world.h"
#include <filesystem>

WorldLoader::WorldLoader(World* world)
{
	this->world = world;
}

bool WorldLoader::loadChunk(const std::shared_ptr<Chunk>& chunk)
{
	std::string worldPath = "saves/" + world->getWorldName();

	WorldRegion region = WorldRegion::toWorldRegion(chunk->getCoord());
	std::string regionPath = saveutil::getRegionPath(region, worldPath);
	if (!std::filesystem::exists(regionPath))
	{
		return false;
	}

	glm::ivec3 relPos = region.getRelativeChunkPosition(chunk->getCoord());
	int32_t lookupOffset = relPos.x + relPos.y * WORLD_REGION_SIZE + relPos.z * WORLD_REGION_SIZE * WORLD_REGION_SIZE;
	if (lookupOffset < 0)
	{
		return false;
	}

	FileReader reader = FileReader(regionPath, false);

	RegionFileHeader header = RegionFileHeader{};
	reader.readRaw(header);
	if (strcmp(header.signature, "VOXEL_REGION") != 0 || header.regionVersion != currentRegionVersion)
	{
		return false;
	}

	reader.setStreamPosition(sizeof(RegionFileHeader) + lookupOffset * sizeof(int32_t));

	int32_t chunkDataOffset = 0;
	reader.readRaw(chunkDataOffset);

	if (chunkDataOffset <= 0)
	{
		return false;
	}

	reader.setStreamPosition(chunkDataOffset);
	ChunkStatus chunkStatus = ChunkStatus::NOT_GENERATED;
	reader.readRaw(chunkStatus);
	reader.readData((char*)chunk->getChunkData(), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * sizeof(BlockType));

	chunk->setChunkStatus(chunkStatus);

	return true;
}
