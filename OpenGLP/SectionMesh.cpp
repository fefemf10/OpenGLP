#include "SectionMesh.hpp"

SectionMesh::SectionMesh()
{
	vao.addVBO(4);
	vaot.addVBO(4);
	vao.setPropertyBuffer(0, 3, GL_FLOAT);
	vao.setPropertyBuffer(1, 3, GL_FLOAT);
	vao.setPropertyBuffer(2, 3, GL_FLOAT);
	vao.setPropertyBuffer(3, 1, GL_UNSIGNED_BYTE);
	vaot.setPropertyBuffer(0, 3, GL_FLOAT);
	vaot.setPropertyBuffer(1, 3, GL_FLOAT);
	vaot.setPropertyBuffer(2, 3, GL_FLOAT);
	vaot.setPropertyBuffer(3, 1, GL_UNSIGNED_BYTE);
}

void SectionMesh::load(const std::vector<glm::vec3>& vertex, const std::vector<glm::vec3>& color, const std::vector<glm::vec3>& UV, const std::vector<uint8_t>& AO, const std::vector<GLuint>& indicies, bool t)
{
	if (!t)
	{
		vao.loadData(0, vertex);
		vao.loadData(1, color);
		vao.loadData(2, UV);
		vao.loadData(3, AO);
		vao.loadIndices(indicies);
		countVertex = vertex.size();
	}
	else
	{
		vaot.loadData(0, vertex);
		vaot.loadData(1, color);
		vaot.loadData(2, UV);
		vaot.loadData(3, AO);
		vaot.loadIndices(indicies);
		countVertext = vertex.size();
	}
}

void SectionMesh::clear()
{
	countVertex = 0;
	countVertext = 0;
	vao.clear();
	vaot.clear();
}

void SectionMesh::draw() const
{
	if (countVertex)
		vao.draw();
}
void SectionMesh::drawt() const
{
	if (countVertext)
		vaot.draw();
}

size_t SectionMesh::getCountVertex() const noexcept
{
	return countVertex;
}

size_t SectionMesh::getCountVertext() const noexcept
{
	return countVertext;
}
