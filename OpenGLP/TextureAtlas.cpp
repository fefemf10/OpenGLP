#include "TextureAtlas.hpp"
#include <fstream>
#include <JSON/json.hpp>

TextureAtlas::TextureAtlas(GLsizei mipmaps, const glm::ivec3& size, GLenum sizedFormat) : Texture(mipmaps, size, sizedFormat), index{ 0 }, t{}
{
}

TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
{
	this->size = other.size;
	this->id = other.id;
	this->sizedFormat = other.sizedFormat;
	this->target = other.target;
	this->mipmaps = other.mipmaps;
	other.target = other.mipmaps = other.sizedFormat = other.id = 0;
	other.size = { 0, 0, 0 };
	this->t = other.t;
	this->index = other.index;
	other.t.clear();
	other.index = 0;
}

TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other) noexcept
{
	glDeleteTextures(1, &id);
	this->size = other.size;
	this->id = other.id;
	this->sizedFormat = other.sizedFormat;
	this->target = other.target;
	this->mipmaps = other.mipmaps;
	other.target = other.mipmaps = other.sizedFormat = other.id = 0;
	other.size = { 0, 0, 0 };
	this->t = other.t;
	this->index = other.index;
	other.t.clear();
	other.index = 0;
	return *this;
}

TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::loadTexture(const std::string& name, const std::string& path)
{
	int bpp{}, width{}, height{}, format{};
	if (name.ends_with("_flow"))
		return;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
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
	if (height > width)
	{
		using nlohmann::json;
		json j;
		std::ifstream mcmeta(path + ".mcmeta");
		mcmeta >> j;
		mcmeta.close();
		j = j["animation"];
		ta[name].width = width;
		ta[name].height = width;
		int countFrames = height / width;
		int ft = 1;
		if (j.contains("frametime"))
			ft = j["frametime"];
		if (j.contains("frames"))
		{
			for (auto& item : j["frames"])
			{
				if (item.is_object())
				{
					ta[name].frames.push_back(Animation::Frame{ .index = item["index"], .time = item["time"] });
				}
				else
					ta[name].frames.push_back(Animation::Frame{ .index = item, .time = ft });
			}
		}
		else
		{
			for (int i = 0; i < countFrames; ++i)
			{
				ta[name].frames.push_back(Animation::Frame{ .index = i, .time = ft });
			}
		}
		glTextureSubImage3D(id, 0, 0, 0, index, width, width, countFrames, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
		ta[name].frameID = index + countFrames;
		ta[name].startFrameID = index;
		glCopyImageSubData(id, GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, id, GL_TEXTURE_2D_ARRAY, 0, 0, 0, index + countFrames, width, width, 1);
		index += countFrames;
		ta[name].idTexture = id;
	}
	else
	{
		if ((width > this->size.x) || (height > this->size.y) || (index > this->size.z))
		{
			stbi_image_free(data);
			return;
		}
		if (target == GL_TEXTURE_2D)
			glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
		else if (target == GL_TEXTURE_2D_ARRAY)
			glTextureSubImage3D(id, 0, 0, 0, index, width, height, 1, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
		t[name] = index;
	}
	++index;
	stbi_image_free(data);
}

const GLsizei& TextureAtlas::at(const std::string& name) const
{
	if (ta.contains(name))
		return ta.at(name).frameID;
	return t.at(name);
}


GLsizei& TextureAtlas::operator[](const std::string& name)
{
	if (ta.contains(name))
		return ta[name].frameID;
	return t[name];
}

void TextureAtlas::updateTick()
{
	for (auto& [key, value] : ta)
	{
		value.tick();
	}
}

void Animation::tick()
{
	++frametime;
	if (frametime >= frames[currentFrame].time)
	{
		frametime = 0;
		currentFrame = ++currentFrame % frames.size();
		currentFrameID = startFrameID + frames[currentFrame].index;
		glCopyImageSubData(idTexture, GL_TEXTURE_2D_ARRAY, 0, 0, 0, currentFrameID, idTexture, GL_TEXTURE_2D_ARRAY, 0, 0, 0, frameID, width, width, 1);
	}
}