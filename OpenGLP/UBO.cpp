#include "UBO.hpp"

UBO::UBO(size_t size, GLuint index) : size(size), index(index)
{
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, size, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
}

void UBO::bind() const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
}

void UBO::unbind() const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
}

UBO::~UBO()
{
	glDeleteBuffers(1, &id);
}