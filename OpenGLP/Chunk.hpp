#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <tuple>
#include <GLM/glm.hpp>
#include "Section.hpp"
#include "Noise.hpp"
#include "VAO.hpp"
#include "Frustum.hpp"

class Chunk
{
public:
	Chunk(std::vector<std::vector<Chunk*>>& chunks);
	void genMesh();
	void draw(const Frustum& frustum);
	void drawTransperent(const Frustum& frustum);
	void findNeighbors();
	Section* getSection(int32_t position);
	glm::ivec3 position{};
	glm::ivec2 localPosition{};
	double seed{};
	std::vector<std::vector<Chunk*>>& chunks;
	std::vector<Section> sections;
	std::vector<int64_t> motion_blocking;
	std::vector<int64_t> motion_blocking_no_leaves;
	std::vector<int64_t> ocean_floor;
	std::vector<int64_t> world_surface;
	bool modified{};
	GLuint countVertex{};
	GLuint countVertexTransperent{};
	uint8_t countVisibleSection{};
private:
	//unsigned char noise(unsigned char countOctaves, double x, double y, double z, double persistence, double scale, unsigned char min, unsigned char max);
};