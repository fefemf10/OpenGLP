#pragma once
#include <vector>
#include <GLEW/glew.h>

class SSBO
{
public:
	SSBO(size_t size, GLuint index);
	template<typename T>
	void loadData(size_t offset, const std::vector<T>& data);
	template<typename T>
	void loadData(size_t offset, const T& data);
	template<typename T>
	void getData(std::vector<T>& data);
	void bind() const;
	void unbind() const;
	~SSBO();
private:
	GLuint id;
	GLuint index;
	size_t size;
};

template<typename T>
inline void SSBO::loadData(size_t offset, const std::vector<T>& data)
{
	glNamedBufferSubData(id, offset, data.size() * sizeof(T), data.data());
}

template<typename T>
inline void SSBO::loadData(size_t offset, const T& data)
{
	glNamedBufferSubData(id, offset, sizeof(T), &data);
}

template<typename T>
inline void SSBO::getData(std::vector<T>& data)
{
	glGetNamedBufferSubData(id, 0, data.size() * sizeof(T), data.data());
}