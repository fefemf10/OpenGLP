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

void VAO::setPropertyBuffer(const GLuint buffer, const GLint size, const GLenum type)
{
	buffers[buffer].attibSize = size;
	buffers[buffer].attibType = type;
	if (VBO::isIntegerType(type))
		glVertexArrayAttribIFormat(vao, buffer, size, type, 0);
	else
		glVertexArrayAttribFormat(vao, buffer, size, type, GL_FALSE, 0);
}

void VAO::clear()
{
	for (size_t i = 0; i < buffers.size(); i++)
	{
		glDeleteBuffers(1, &buffers[i].id);
		glCreateBuffers(1, &buffers[i].id);
		glNamedBufferStorage(buffers[i].id, buffers[i].size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		setPropertyBuffer(i, buffers[i].attibSize, buffers[i].attibType);
		if (buffers[i].attibSize > 0)
			glVertexArrayVertexBuffer(vao, i, buffers[i].id, 0, buffers[i].attibSize * VBO::getSizeAttribute(buffers[i].attibType));
		buffers[i].size = 0u;
	}
	indicesCount = 0;
}

void VAO::copyAllVBO(const VAO& other)
{
	buffers = other.buffers;
	indicesCount = other.indicesCount;
	for (size_t i = 0; i < buffers.size(); i++)
	{
		glCreateBuffers(1, &buffers[i].id);
		if (buffers[i].size > 0)
			glNamedBufferStorage(buffers[i].id, buffers[i].size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		setPropertyBuffer(i, buffers[i].attibSize, buffers[i].attibType);
		if (buffers[i].attibSize > 0)
			glVertexArrayVertexBuffer(vao, i, buffers[i].id, 0, buffers[i].attibSize * VBO::getSizeAttribute(buffers[i].attibType));
		if (buffers[i].size > 0)
		glCopyNamedBufferSubData(other.buffers[i].id, buffers[i].id, 0, 0, buffers[i].size);
	}
	glCreateBuffers(1, &ibo);
	if (indicesCount > 0)
	{
		glNamedBufferStorage(ibo, indicesCount * VBO::getSizeAttribute(typeIndices), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glCopyNamedBufferSubData(other.ibo, ibo, 0, 0, indicesCount * VBO::getSizeAttribute(typeIndices));
	}
	glVertexArrayElementBuffer(vao, ibo);
}
