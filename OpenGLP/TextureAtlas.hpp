#pragma once
#include <unordered_map>
#include <vector>
#include "Texture.hpp"

struct Animation
{
	struct Frame
	{
		int index{};
		int time{};
	};
	bool interpolate{};
	int width{};
	int height{};
	std::vector<Frame> frames;
	int frametime{};
	int currentFrame{};
	GLuint idTexture{};
	GLsizei frameID{};
	GLsizei currentFrameID{};
	GLsizei startFrameID{};
	void tick();
};

class TextureAtlas : public Texture
{
public:
	TextureAtlas() = default;
	TextureAtlas(GLsizei mipmaps, const glm::ivec3& size, GLenum sizedFormat);
	TextureAtlas(const TextureAtlas& other) = delete;
	TextureAtlas& operator=(const TextureAtlas& other) = delete;
	TextureAtlas(TextureAtlas&& other) noexcept;
	TextureAtlas& operator=(TextureAtlas&& other) noexcept;
	virtual ~TextureAtlas();
	void loadTexture(const std::string& name, const std::string& path);
	const GLsizei& at(const std::string& name) const;
	GLsizei& operator[](const std::string& name);
	void updateTick();
private:
	std::unordered_map<std::string, GLsizei> t;
	std::unordered_map<std::string, Animation> ta;
	GLsizei index{};
};

