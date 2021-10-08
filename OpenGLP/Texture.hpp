#pragma once
#include <string>
#include <GLM/ext/vector_int3.hpp>
#include <GLM/ext/vector_int2.hpp>
#include <GLEW/glew.h>
#include "stb_image.hpp"
class Texture
{
public:
	Texture();
	Texture(GLsizei mipmaps, const glm::ivec3& size, GLenum sizedFormat);
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	virtual ~Texture();
	void editTexture(const glm::ivec3& offset, const glm::ivec2& size, GLenum format, GLenum type, const char* buffer);
	void editTexture(const std::string& name, GLsizei z = 0);
	void setRepeat(bool value);
	void setSmooth(bool value);
	void generateMipmaps() const;
	GLuint getID() const noexcept;
	GLenum getTarget() const noexcept;
	GLenum getSizedFormat() const noexcept;
	GLsizei getMipmaps() const noexcept;
	void bind(GLuint unit) const;
	void unbind(GLuint unit) const;
protected:
	glm::ivec3 size{};
	GLuint id{};
	GLenum target{};
	GLenum sizedFormat{};
	GLsizei mipmaps{};
};