#pragma once
#include <filesystem>
#include <fstream>
#include <random>
#include <vector>
#include <thread>
#include <atomic>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "../IMGUI/imgui.h"
#include "queuep.hpp"
#include "ThreadPool.hpp"
#include "Camera.hpp"
#include "Chunk.hpp"
#include "Region.hpp"
#include "SectionMesh.hpp"
#include "Enums.hpp"
#include "Dispatcher.hpp"
#include "ChunkObserver.hpp"
#include "WorldInfo.hpp"

class World
{
public:
	World(Player& player, Camera& camera);
	void saveWorld();
	std::string getBlock(const glm::ivec3& position);
	Chunk* getChunk(const glm::ivec2& position);
	Section* getSection(const glm::ivec3& position);
	void setBlock(const glm::ivec3& position, const std::string& id);
	void loadChunks();
	uint64_t getSeed() const;
	glm::ivec2 getLocalPositionChunk(const glm::ivec2& position) const;
	bool validateLocalPosChunk(const glm::ivec2& position) const;
	unsigned short getCountChunks();
	std::string rayCast(const glm::vec3& pos, const glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);
	void update(const float& dt);
	void updateTick();
	void draw();
	unsigned short countVisibleChunks;
	unsigned int allVertex;
	GLuint countVertex;
	GLuint countVertexTransperent;
	uint32_t countVisibleSection;
	std::vector<std::vector<Chunk*>> chunks;
	std::vector<std::vector<std::vector<SectionMesh*>>> sectionMesh;
	queuep genMeshq;
	bool loadedNeighbours(const glm::ivec2& position);
private:
	
	
	glm::uvec2 endLocalPosChunk;
	ThreadPool poolChunks;
	Player& player;
	Camera& camera;
	WorldInfo& worldInfo;
	glm::ivec2 playerLastPosition;
	bool firstLoad = true;
	uint64_t seed;
	unsigned short countChunks;
	float tick{};
	ChunkObserver chunkObserver;
	Dispatcher dispatcher;
};