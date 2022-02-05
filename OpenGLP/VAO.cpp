#include "VAO.hpp"

VAO::VAO()
{
	glCreateVertexArrays(1, &vao);
}

VAO::VAO(const VAO& other)
{
	glCreateVertexArrays(1, &vao);
	addVBO(other.buffers.size());
	copyAllVBO(other);
	indicesCount = other.indicesCount;
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

void VAO::clear()
{
	for (size_t i = 0; i < buffers.size(); i++)
	{
		glDeleteBuffers(1, &buffers[i].first);
		buffers[i].second = 0u;
	}
	indicesCount = 0;
}

void VAO::copyAllVBO(const VAO& other)
{
	for (size_t i = 0; i < other.buffers.size(); i++)
	{
		glCreateBuffers(1, &buffers[i].first);
		glNamedBufferStorage(buffers[i].first, other.buffers[i].second, nullptr, 0);
		glEnableVertexArrayAttrib(vao, i);
		glVertexArrayAttribBinding(vao, i, i);
		if (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT || type == GL_BYTE || type == GL_SHORT || type == GL_INT)
			glVertexArrayAttribIFormat(vao, i, 1, type, 0);
		else
			glVertexArrayAttribFormat(vao, i, sizeof(T) / sizeof(GL_FLOAT), type, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, i, buffers[buffer].first, 0, sizeof(T));
		//glCopyNamedBufferSubData(other.buffers[i].first, buffers[i].first, 0, 0, other.buffers[i].second);
		buffers[i].second = other.buffers[i].second;
	}
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, other.indicesCount * (other.typeIndices == GL_UNSIGNED_INT ? 4 : 1), nullptr, 0);
	glCopyNamedBufferSubData(other.ibo, ibo, 0, 0, other.indicesCount * (other.typeIndices == GL_UNSIGNED_INT ? 4 : 1 ));
	glVertexArrayElementBuffer(vao, ibo);

}
