#pragma once
#include <atomic>
#include "Section.hpp"

class Chunk
{
public:
	Chunk(std::vector<std::vector<Chunk*>>& chunks);
	void genMesh();
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
	std::atomic<bool> modified{};
	std::atomic<bool> work{};
	uint8_t countVisibleSection{};
private:
	//unsigned char noise(unsigned char countOctaves, double x, double y, double z, double persistence, double scale, unsigned char min, unsigned char max);
};