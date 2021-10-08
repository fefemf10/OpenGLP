#pragma once
#include <vector>
#include <array>
#include <GLM/glm.hpp>
#include "Block.hpp"
#include "VAO.hpp"


struct PaletteItem
{
	Enums::Block id{};
	std::map<Enums::PropertiesBlock, uint8_t> properties;
};

struct Section
{
	static PaletteItem stone;
	Section();
	template<typename T>
	inline T swapEndian(T value)
	{
		T tmp;
		for (size_t i = 0; i < sizeof(T); ++i)
		{
			char tmpc = reinterpret_cast<char*>(&tmp)[sizeof(T) - i - 1];
			reinterpret_cast<char*>(&tmp)[sizeof(T) - i - 1] = reinterpret_cast<char*>(&value)[i];
			reinterpret_cast<char*>(&value)[i] = tmpc;
		}
		return tmp;
	}
	std::vector<int8_t> blockLight;
	std::vector<int8_t> skyLight;
	std::vector<int64_t> dataBlock;
	std::vector<int64_t> dataBiome;
	std::vector<PaletteItem> blockPalette;
	std::vector<Enums::Biome> biomePalette;
	Section* neighbors[6]{};
	/*5
	  1
	2   3
	  4
	  0*/
	int16_t height{};
	const PaletteItem& getBlock(const glm::i8vec3& blockPos);
	uint8_t bitsPerBlock{};
	uint8_t bitsPerBiome{};
	uint8_t countIndexPerLongBlock{};
	uint8_t countIndexPerLongBiome{};
	int64_t maskBlock{};
	int64_t maskBiome{};
	void gen(uint64_t seed, const glm::vec2& chunkPos);
	GLuint countVertex{};
	GLuint countVertexTransperent{};
	bool modified{};
	bool buffer{};
	bool visible{};
	void genMesh(const glm::ivec2& position);
	void loadBuffer();
	void draw();
	void drawTransperent();
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> color;
	std::vector<glm::vec3> UV;
	std::vector<GLuint> indicies;
	std::vector<glm::vec3> vertext;
	std::vector<glm::vec3> colort;
	std::vector<glm::vec3> UVt;
	std::vector<GLuint> indiciest;
	VAO vao;
	VAO vaoTransperent;
};