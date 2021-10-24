#pragma once
#include <filesystem>
#include <fstream>
#include <random>
#include <thread>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "../IMGUI/imgui.h"
#include "queuep.hpp"
#include "ThreadPool.hpp"
#include "Camera.hpp"
#include "Chunk.hpp"
#include "Region.hpp"
#include "NBTFile.hpp"
#include "nbt.hpp"
class World
{
public:
	World(const std::string& name, Player& player, Camera& camera);
	void saveWorld();
	std::string getBlock(const glm::ivec3& position);
	Chunk* getChunk(const glm::ivec2& position);
	Section* getSection(const glm::ivec3& position);
	void setBlock(const glm::ivec3& position, const std::string& id);
	void loadChunks();
	uint64_t getSeed() const;
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
private:
	bool validateLocalPosChunk(const glm::ivec2& position);
	glm::uvec2 endLocalPosChunk;
	ThreadPool poolChunks;
	queuep loadq;
	queuep drawq;
	std::vector<std::vector<Chunk*>> chunks;
	Player& player;
	Camera& camera;
	glm::ivec2 playerLastPosition;
	std::filesystem::path worldDir;
	bool firstLoad = true;
	uint64_t seed;
	nbt::NBTFile level;
	std::unordered_map<glm::ivec2, nbt::Region> regions;
	unsigned short countChunks;
	float tick{};
};