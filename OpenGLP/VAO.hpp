#pragma once
#include <vector>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
class VAO
{
public:
	struct VBO
	{
		GLuint id;
		uint32_t size;
		GLenum attibType;
		uint32_t attibSize;
		static bool isIntegerType(GLenum type) noexcept
		{
			switch (type)
			{
			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
			case GL_INT:
			case GL_UNSIGNED_INT:
				return true;
			default:
				return false;
			}
		}
		static uint8_t getSizeAttribute(GLenum type) noexcept
		{
			switch (type)
			{
			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
				return 1u;
			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
				return 2u;
			case GL_INT:
			case GL_UNSIGNED_INT:
			case GL_FLOAT:
				return 4u;
			case GL_DOUBLE:
				return 8u;
			}
		}
	};
	VAO();
	VAO(const VAO& other);
	VAO(VAO&& other) noexcept;
	~VAO();
	void bind() const;
	void draw(const GLenum& mode = GL_TRIANGLES) const;
	void addVBO(GLuint count);
	template <typename T>
	void loadIndices(const std::vector<T>& data);
	template <typename T>
	void loadData(const GLuint buffer, const std::vector<T>& data);
	void setTypeIndices(const GLenum type);
	void setPropertyBuffer(const GLuint buffer, const GLint size, const GLenum type);
	void clear();
private:
	void copyAllVBO(const VAO& other);
	GLuint vao{ 0 }, ibo{ 0 }, indicesCount{ 0 };
	GLenum typeIndices = GL_UNSIGNED_INT;
	std::vector<VBO> buffers;
};

template<typename T>
inline void VAO::loadIndices(const std::vector<T>& data)
{
	if (data.size() != indicesCount)
	{
		glDeleteBuffers(1, &ibo);
		glCreateBuffers(1, &ibo);
		indicesCount = static_cast<GLuint>(data.size());
		glNamedBufferStorage(ibo, data.size() * sizeof(T), data.data(), GL_DYNAMIC_STORAGE_BIT);
	}
	else
	{
		glNamedBufferSubData(ibo, 0, data.size() * sizeof(T), data.data());
	}
	glVertexArrayElementBuffer(vao, ibo);
}

template<typename T>
inline void VAO::loadData(const GLuint buffer, const std::vector<T>& data)
{
	if (data.size() != buffers[buffer].size)
	{
		glDeleteBuffers(1, &buffers[buffer].id);
		glCreateBuffers(1, &buffers[buffer].id);
		buffers[buffer].size = static_cast<GLuint>(data.size());
		glNamedBufferStorage(buffers[buffer].id, data.size() * sizeof(T), data.data(), GL_DYNAMIC_STORAGE_BIT);
		if (buffers[buffer].attibSize > 0)
			glVertexArrayVertexBuffer(vao, buffer, buffers[buffer].id, 0, buffers[buffer].attibSize * VBO::getSizeAttribute(buffers[buffer].attibType));
	}
	else
	{
		glNamedBufferSubData(buffers[buffer].id, 0, data.size() * sizeof(T), data.data());
	}
}