#pragma once
#include "VAO.hpp"
class SectionMesh
{
public:
	SectionMesh();
	SectionMesh(const SectionMesh& other) = default;
	void init();
	void load(const std::vector<glm::vec3>& vertex, const std::vector<glm::vec3>& color, const std::vector<glm::vec3>& UV, const std::vector<uint8_t>& AO, const std::vector<GLuint>& indicies, bool t = false);
	void clear();
	void draw() const;
	void drawt() const;
	size_t getCountVertex() const noexcept;
	size_t getCountVertext() const noexcept;
	glm::ivec3 position;
private:
	VAO vao;
	VAO vaot;
	size_t countVertex{};
	size_t countVertext{};
};

