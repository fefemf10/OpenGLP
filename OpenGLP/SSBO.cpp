#include "SSBO.hpp"

SSBO::SSBO(size_t size, GLuint index) : size(size), index(index)
{
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, size, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
}

void SSBO::bind() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
}

void SSBO::unbind() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
}

SSBO::~SSBO()
{
	glDeleteBuffers(1, &id);
}