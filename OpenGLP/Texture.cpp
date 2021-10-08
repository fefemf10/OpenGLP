#include "Texture.hpp"

Texture::Texture()
{
}

Texture::Texture(GLsizei mipmaps, const glm::ivec3& size, GLenum sizedFormat) : mipmaps(mipmaps), size(size), sizedFormat(sizedFormat)
{
	if (size.x != 0 && size.y != 0 && size.z == 0)
	{
		target = GL_TEXTURE_2D;
		glCreateTextures(target, 1, &id);
		glTextureStorage2D(id, mipmaps, sizedFormat, size.x, size.y);
	}
	else if (size.x != 0 && size.y != 0 && size.z != 0)
	{
		target = GL_TEXTURE_2D_ARRAY;
		glCreateTextures(target, 1, &id);
		glTextureStorage3D(id, mipmaps, sizedFormat, size.x, size.y, size.z);
	}
	setRepeat(true);
	setSmooth(false);
	generateMipmaps();
}

Texture::Texture(Texture&& other) noexcept
{
	this->size = other.size;
	this->id = other.id;
	this->sizedFormat = other.sizedFormat;
	this->target = other.target;
	this->mipmaps = other.mipmaps;
	other.target = other.mipmaps = other.sizedFormat = other.id = 0;
	other.size = { 0, 0, 0 };
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	glDeleteTextures(1, &id);
	this->size = other.size;
	this->id = other.id;
	this->sizedFormat = other.sizedFormat;
	this->target = other.target;
	this->mipmaps = other.mipmaps;
	other.target = other.mipmaps = other.sizedFormat = other.id = 0;
	other.size = { 0, 0, 0 };
	return *this;
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::editTexture(const glm::ivec3& offset, const glm::ivec2& size, GLenum format, GLenum type, const char* buffer)
{
	if (target == GL_TEXTURE_2D)
		glTextureSubImage2D(id, 0, offset.x, offset.y, size.x, size.y, format, type, static_cast<const void*>(buffer));
	else if (target == GL_TEXTURE_2D_ARRAY)
		glTextureSubImage3D(id, 0, offset.x, offset.y, offset.z, size.x, size.y, 1, format, type, static_cast<const void*>(buffer));
}

void Texture::editTexture(const std::string& name, GLsizei z)
{
	//stbi_set_flip_vertically_on_load(true);
	int bpp{}, width{}, height{}, format{};
	unsigned char* data = stbi_load(name.c_str(), &width, &height, &bpp, 0);
	if ((width > this->size.x) || (height > this->size.y) || (z > this->size.z))
	{
		stbi_image_free(data);
		return;
	}
	switch (bpp)
	{
	case 1:
		format = GL_RED;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 2:
		format = GL_RG;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		break;
	case 3:
		format = GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	case 4:
		format = GL_RGBA;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	}
	if (target == GL_TEXTURE_2D)
		glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
	else if (target == GL_TEXTURE_2D_ARRAY)
		glTextureSubImage3D(id, 0, 0, 0, z, width, height, 1, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
	stbi_image_free(data);
}

void Texture::setRepeat(bool value)
{
	if (value)
	{
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void Texture::setSmooth(bool value)
{
	if (value)
	{
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void Texture::generateMipmaps() const
{
	glGenerateTextureMipmap(id);
}

GLuint Texture::getID() const noexcept
{
	return id;
}

GLenum Texture::getTarget() const noexcept
{
	return target;
}

GLenum Texture::getSizedFormat() const noexcept
{
	return sizedFormat;
}

GLsizei Texture::getMipmaps() const noexcept
{
	return mipmaps;
}

void Texture::bind(GLuint unit) const
{
	glBindTextureUnit(unit, id);
}

void Texture::unbind(GLuint unit) const
{
	glBindTextureUnit(unit, 0);
}