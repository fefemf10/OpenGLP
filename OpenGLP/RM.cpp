#include "RM.hpp"

std::unordered_map<std::string, Shader> RM::shaders;
std::unordered_map<std::string, Texture> RM::textures;
std::unordered_map<std::string, TextureAtlas> RM::atlases;

void RM::loadShader(const std::string& name, Shader&& shader)
{
	shaders.try_emplace(name, std::move(shader));
}

void RM::loadTexture(const std::string& name, Texture&& texture)
{
	textures.try_emplace(name, std::move(texture));
}

void RM::loadAtlas(const std::string& name, TextureAtlas&& texture)
{
	atlases.try_emplace(name, std::move(texture));
}

void RM::unloadShader(const std::string& name)
{
	shaders.erase(name);
}

void RM::unloadTexture(const std::string& name)
{
	textures.erase(name);
}

void RM::unloadAtlas(const std::string& name)
{
	atlases.erase(name);
}

Shader& RM::getShader(const std::string& name)
{
	return shaders[name];
}

Texture& RM::getTexture(const std::string& name)
{
	return textures[name];
}

TextureAtlas& RM::getAtlas(const std::string& name)
{
	return atlases[name];
}

uint32_t RM::getIDShader(const std::string& name)
{
	return shaders[name].getID();
}

uint32_t RM::getIDTexture(const std::string& name)
{
	return textures[name].getID();
}
uint32_t RM::getIDAtlas(const std::string& name)
{
	return atlases[name].getID();
}
//void ResourceManager::useShader(const std::string& name)
//{
//	if (currentShader != name)
//	{
//		currentShader = name;
//		shaders[name].use();
//	}
//}
//
//void ResourceManager::useTexture(const std::string& name)
//{
//	if (currentTexture != name)
//	{
//		currentTexture = name;
//		currentAtlas = name;
//		textures[name].bind();
//	}
//}
//
//void ResourceManager::useTexture(GLuint id, GLuint unit)
//{
//	if (idTexture != id)
//	{
//		idTexture = id;
//		currentTexture = "";
//		currentAtlas = "";
//		glBindSampler(unit, GL_SAMPLER_2D);
//		glBindTextureUnit(unit, id);
//		
//		//glBindTextureUnit(unit, id);
//	}
//}
//
//void ResourceManager::useAtlas(const std::string& name)
//{
//	if (currentAtlas != name)
//	{
//		currentAtlas = name;
//		currentTexture = name;
//		atlases[name].bind();
//	}
//}