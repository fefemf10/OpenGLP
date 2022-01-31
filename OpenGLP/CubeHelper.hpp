#pragma once
#include <vector>
#include <array>
#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLEW/glew.h>
#include "Enums.hpp"
#include "TextureAtlas.hpp"
#include "Block.hpp"
#include "PaletteItem.hpp"

struct Section;

class CubeHelper
{
public:
	CubeHelper(Section& section, const PaletteItem& pi, const glm::ivec3& position, const glm::vec3& colorBlock, const glm::ivec3& localPos, const glm::vec2& rotation, const Blocks::Model::Element& item, uint32_t countVertex, const TextureAtlas& atlas, const std::map<Enums::TextureSlot, GLuint>& textures);
	void addQuad(const Enums::Direction d);
	void rotateFunc(const glm::vec4& uv, int angle, GLuint texture);
	void rotateCube(const Enums::Axis& axis, const float& angle, const glm::vec3& origin);
	void vertexPush(const std::array<glm::vec3, 4>& quad);
	void indiciesPush();
	void add(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& UV, std::vector<glm::vec3>& color, std::vector<uint8_t>& AO, std::vector<uint32_t>& indicies, uint32_t& countVertex);
private:
	Section& section;
	const PaletteItem& pi;
	const std::map<Enums::TextureSlot, GLuint>& textures;
	const Blocks::Model::Element& item;
	const TextureAtlas& atlas;
	const glm::ivec3& position;
	const glm::vec3& colorBlock;
	const glm::ivec3& localPos;
	std::array<glm::vec3, 8> cube;
	std::array<std::array<glm::vec3, 4>, 6> cubefaces;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> UV;
	std::vector<glm::vec3> color;
	std::vector<uint8_t> AO;
	std::vector<uint32_t> indicies;
	uint32_t countVertex;
	std::array<std::array<std::array<bool, 3>, 3>, 3> nBlocks;
};