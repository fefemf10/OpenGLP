#pragma once
#include <vector>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
class VAO
{
public:
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
	void loadData(const GLuint buffer, const std::vector<T>& data, GLenum type = GL_FLOAT);
	void setTypeIndices(GLenum type);
	void clear();
private:
	struct VBO
	{
		GLuint id;
		uint32_t size;
		GLenum type;
	};
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
		glNamedBufferStorage(ibo, data.size() * sizeof(T), data.data(), 0);
	}
	else
	{
		glNamedBufferSubData(ibo, 0, data.size() * sizeof(T), data.data());
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vao, ibo);
}

template<typename T>
inline void VAO::loadData(const GLuint buffer, const std::vector<T>& data, GLenum type)
{
	if (data.size() != buffers[buffer].second)
	{
		glDeleteBuffers(1, &buffers[buffer].first);
		glCreateBuffers(1, &buffers[buffer].first);
		buffers[buffer].second = static_cast<GLuint>(data.size());
		glNamedBufferStorage(buffers[buffer].first, data.size() * sizeof(T), data.data(), 0);
	}
	else
	{
		glNamedBufferSubData(buffers[buffer].first, 0, data.size() * sizeof(T), data.data());
	}
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer].first);
	//glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(buffer, sizeof(T) / sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, sizeof(T), nullptr);
	glEnableVertexArrayAttrib(vao, buffer);
	glVertexArrayAttribBinding(vao, buffer, buffer);
	if (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT || type == GL_BYTE || type == GL_SHORT || type == GL_INT)
		glVertexArrayAttribIFormat(vao, buffer, 1, type, 0);
	else
		glVertexArrayAttribFormat(vao, buffer, sizeof(T) / sizeof(GL_FLOAT), type, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, buffer, buffers[buffer].first, 0, sizeof(T));
}