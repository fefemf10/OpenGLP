#pragma once
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <gtc/noise.hpp>
#include <detail/type_half.hpp>
#include <random>
#include <ext\matrix_transform.hpp>
#include <glm.hpp>
#include <lz4.h>
#include "VAO.hpp"
#include "Block.hpp"
#include "Program.hpp"

class Chunk
{
public:
	Chunk(std::vector<std::vector<Chunk*>>& chunks);
	~Chunk();
	void generateChunk(const glm::ivec2& position, const double& seed);
	bool loadChunk(const glm::ivec2& position, const std::filesystem::path& path);
	void saveChunk(const std::filesystem::path& path);
	void setBlock(const glm::u8vec3& position, const Block::Type& type);
	Block& getBlock(const glm::u8vec3& position);
	const glm::ivec2& getPosition() const;
	const unsigned char& getMaxHeightBlock() const;
	unsigned int getCountVertexs();
	void findNeighbors();
	void setModified(const bool& value);
	std::vector<glm::u8vec2>& getNeighbors();
	void generateMesh();
	void draw(GL::Program& shader);
	bool saving = false;
private:
	GLuint countVertex;
	bool isTB(const glm::i16vec3& position);
	glm::ivec2 position;
	bool modified;
	unsigned char maxHeightBlock;
	Block blocks[256][16][16];
	GL::VAO vao;
	std::vector<std::vector<Chunk*>>& chunks;
	std::vector<glm::u8vec2>idChunks;
};
