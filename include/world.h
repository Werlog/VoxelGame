#pragma once

#include "chunk.h"
#include <unordered_map>
#include "shader.h"
#include <FastNoiseSIMD.h>
#include <queue>
#include "player.h"
#include <mutex>
#include "chunkmanager.h"
#include "saving/worldsaver.h"


class PlayingGameState;

class World
{
public:
	static constexpr int RENDER_DISTANCE = 10;
	static constexpr int SIMULATION_DISTANCE = 1; // The chunk radius in which blocks will receive random updates
	static constexpr float RANDOM_UPDATE_DELAY = 0.1f;
	static constexpr int RENDER_HEIGHT = 1;
	static constexpr int MAX_UNLOAD_COUNT = 2; // Maximum amount of chunks to unload per frame
	static constexpr int worldDirections[6][3] = { { 1, 0, 0 }, { 0, 0, 1 }, { -1, 0, 0, }, { 0, 0, -1 }, { 0, 1, 0 }, { 0, -1, 0 } };

	World(Shader& terrainShader, PlayingGameState* playingState, const std::string& worldName, int worldSeed);

	void updateLoadedChunks(ChunkCoord& newCoordinate);
	void loadChunk(ChunkCoord coordinate);
	void remeshChunk(ChunkCoord coordinate);

	void updateWorld(Player& player, float deltaTime);
	void renderWorld(const ChunkCoord& playerCoord, Camera& camera);

	BlockType getBlockAt(int x, int y, int z);
	void setBlockAt(int x, int y, int z, BlockType newBlock);
	void setBlockAtForce(int x, int y, int z, BlockType newBlock);
	void modifyBlockAt(int x, int y, int z, BlockType newBlock, bool showParticles = false, bool updateNeighbours = true);

	ChunkManager& getChunkManager();
	const std::string& getWorldName();

	void saveWorld();
	
	std::shared_ptr<Chunk> getChunkByCoordinate(ChunkCoord coord);
	bool isChunkLoaded(ChunkCoord coord);
	FastNoiseSIMD* getNoise();
	FastNoiseSIMD* getTreeNoise();
	FastNoiseSIMD* getWeirdnessNoise();
	FastNoiseSIMD* getGrassNoise();
	int getWorldSeed();
	BlockData& getBlockData();
private:
	PlayingGameState* playingState;

	float sinceRandomUpdate;

	BlockData blockData;
	ChunkManager chunkManager;
	int seed;
	std::string worldName;

	WorldSaver worldSaver;

	ChunkCoord lastplayerCoord;

	unsigned int shaderModelLoc;
	FastNoiseSIMD* noise;
	FastNoiseSIMD* weirdnessNoise;
	FastNoiseSIMD* treeDensityNoise;
	FastNoiseSIMD* grassDensityNoise;

	void setupWorldGen();
	void performRandomUpdates(float deltaTime);
};
