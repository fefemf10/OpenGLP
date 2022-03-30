#include "SectionMesh.hpp"

SectionMesh::SectionMesh()
{
	
}

void SectionMesh::init()
{
	vao.addVAO();
	//vaot.addVAO();
	vao.addVBO(4);
	//vaot.addVBO(4);
	vao[0].attibSize = 3;
	vao[0].attibType = GL_FLOAT;
	vao[1].attibSize = 3;
	vao[1].attibType = GL_FLOAT;
	vao[2].attibSize = 3;
	vao[2].attibType = GL_FLOAT;
	vao[3].attibSize = 1;
	vao[3].attibType = GL_UNSIGNED_BYTE;
	/*vaot[0].attibSize = 3;
	vaot[0].attibType = GL_FLOAT;
	vaot[1].attibSize = 3;
	vaot[1].attibType = GL_FLOAT;
	vaot[2].attibSize = 3;
	vaot[2].attibType = GL_FLOAT;
	vaot[3].attibSize = 1;
	vaot[3].attibType = GL_UNSIGNED_BYTE;*/
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
	/*else
	{
		vaot.loadData(0, vertex);
		vaot.loadData(1, color);
		vaot.loadData(2, UV);
		vaot.loadData(3, AO);
		vaot.loadIndices(indicies);
		countVertext = vertex.size();
	}*/
}

void SectionMesh::clear()
{
	countVertex = 0;
	countVertext = 0;
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
