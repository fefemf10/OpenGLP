#pragma once
#include <unordered_map>
#include <filesystem>
#include "Shader.hpp"
#include "TextureAtlas.hpp"

namespace paths
{
	static std::filesystem::path assets("assets");
	static std::filesystem::path minecraft("minecraft");
	static std::filesystem::path blockstates("blockstates");
	static std::filesystem::path json("json");
	static std::filesystem::path models("models");
	static std::filesystem::path shaders("shaders");
	static std::filesystem::path textures("textures");
};

class RM
{
public:
	static void loadShader(const std::string& name, Shader&& shader);
	static void loadTexture(const std::string& name, Texture&& texture);
	static void loadAtlas(const std::string& name, TextureAtlas&& texture);
	static void unloadShader(const std::string& name);
	static void unloadTexture(const std::string& name);
	static void unloadAtlas(const std::string& name);
	static Shader& getShader(const std::string& name);
	static Texture& getTexture(const std::string& name);
	static TextureAtlas& getAtlas(const std::string& name);
	static uint32_t getIDShader(const std::string& name);
	static uint32_t getIDTexture(const std::string& name);
	static uint32_t getIDAtlas(const std::string& name);
	//static void useShader(const std::string& name);
	//static void useTexture(const std::string& name);
	//static void useTexture(GLuint id, GLuint unit = GL_TEXTURE0);
	//static void useAtlas(const std::string& name);
private:
	static std::unordered_map<std::string, Shader> shaders;
	static std::unordered_map<std::string, Texture> textures;
	static std::unordered_map<std::string, TextureAtlas> atlases;
};
