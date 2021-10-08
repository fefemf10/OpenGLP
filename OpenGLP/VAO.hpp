#pragma once
#include <vector>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
class VAO
{
public:
	VAO();
	VAO(const VAO& other) = delete;
	VAO(VAO&& other) noexcept;
	~VAO();
	void bind() const;
	void draw(const GLenum& mode = GL_TRIANGLES) const;
	void addVBO(GLuint count);
	template <typename T>
	void loadIndices(const std::vector<T>& data);
	template <typename T>
	void loadData(const GLuint& buffer, const std::vector<T>& data);
	void setTypeIndices(GLenum type);
private:
	GLuint vao{ 0 }, ibo{ 0 }, indicesCount{ 0 };
	GLenum typeIndices = GL_UNSIGNED_INT;
	std::vector<std::pair<GLuint, uint32_t>> buffers;
};

template<typename T>
inline void VAO::loadIndices(const std::vector<T>& data)
{
	if (data.size() != indicesCount)
	{
		glDeleteBuffers(1, &ibo);
		glCreateBuffers(1, &ibo);
		indicesCount = static_cast<GLuint>(data.size());
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	glNamedBufferStorage(ibo, data.size() * sizeof(T), data.data(), 0);
	glVertexArrayElementBuffer(vao, ibo);
}

template<typename T>
inline void VAO::loadData(const GLuint& buffer, const std::vector<T>& data)
{
	if (data.size() != buffers[buffer].second)
	{
		glDeleteBuffers(1, &buffers[buffer].first);
		glCreateBuffers(1, &buffers[buffer].first);
		buffers[buffer].second = static_cast<GLuint>(data.size());
	}
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer].first);
	//glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	glNamedBufferStorage(buffers[buffer].first, data.size() * sizeof(T), data.data(), 0);
	//glVertexAttribPointer(buffer, sizeof(T) / sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, sizeof(T), nullptr);
	glEnableVertexArrayAttrib(vao, buffer);
	glVertexArrayAttribBinding(vao, buffer, buffer);
	glVertexArrayAttribFormat(vao, buffer, sizeof(T) / sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, buffer, buffers[buffer].first, 0, sizeof(T));
}