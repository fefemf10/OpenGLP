#include "VAO.hpp"

VAO::VAO()
{
	glCreateVertexArrays(1, &vao);
}

VAO::VAO(VAO&& other) noexcept
{
	for (size_t i = 0; i < buffers.size(); ++i)
		glDeleteBuffers(1, &buffers[i].first);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	buffers.clear();
	vao = other.vao;
	other.vao = 0;
	ibo = other.ibo;
	other.ibo = 0;
	buffers = other.buffers;
	other.buffers.clear();
	indicesCount = other.indicesCount;
	other.indicesCount = 0;
}

VAO::~VAO()
{
	/*glDeleteBuffers(buffers.size(), buffers.data());*/
	for (size_t i = 0; i < buffers.size(); ++i)
		glDeleteBuffers(1, &buffers[i].first);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	ibo = 0;
	vao = 0;
}

void VAO::bind() const
{
	glBindVertexArray(vao);
}

void VAO::draw(const GLenum& mode) const
{
	glBindVertexArray(vao);
	glDrawElements(mode, indicesCount, typeIndices, nullptr);
}

void VAO::addVBO(GLuint count)
{
	buffers.resize(buffers.size() + count, { 0, 0 });
}

void VAO::setTypeIndices(GLenum type)
{
	typeIndices = type;
}