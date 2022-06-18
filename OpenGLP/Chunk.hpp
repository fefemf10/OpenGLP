#pragma once
#include "Section.hpp"
#include "State.hpp"

class Chunk
{
public:
	void genMesh();
	Section* getSection(int32_t position);
	bool isModified() const;
	bool isWork() const;
	void setModified(bool value);
	void setWork(bool value);
	glm::ivec3 position{};
	glm::ivec2 localPosition{};
	double seed{};
	std::vector<Section> sections;
	std::vector<int64_t> motion_blocking;
	std::vector<int64_t> motion_blocking_no_leaves;
	std::vector<int64_t> ocean_floor;
	std::vector<int64_t> world_surface;
	std::atomic<bool> work{};
	std::atomic<State> state{};
	uint8_t countVisibleSection{};
	uint8_t sectionCount{};
private:
	//unsigned char noise(unsigned char countOctaves, double x, double y, double z, double persistence, double scale, unsigned char min, unsigned char max);
};