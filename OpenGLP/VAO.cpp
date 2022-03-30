#include "VAO.hpp"

VAO::VAO()
{
}

VAO::VAO(VAO&& other) noexcept
{
	for (size_t i = 0; i < buffers.size(); ++i)
		glDeleteBuffers(1, &buffers[i].id);
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
		glDeleteBuffers(1, &buffers[i].id);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
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

void VAO::addVAO()
{
	glCreateVertexArrays(1, &vao);
}

void VAO::addVBO(GLuint count)
{
	buffers.resize(buffers.size() + count, { 0, 0 });
	for (size_t i = 0; i < buffers.size(); i++)
	{
		glEnableVertexArrayAttrib(vao, i);
		glVertexArrayAttribBinding(vao, i, i);
	}
}

void VAO::setTypeIndices(const GLenum type)
{
	typeIndices = type;
}

void VAO::clear()
{
	for (size_t i = 0; i < buffers.size(); i++)
	{
		buffers[i].size = 0u;
	}
	indicesCount = 0;
}

VAO::VBO& VAO::operator[](size_t index)
{
	return buffers[index];
}