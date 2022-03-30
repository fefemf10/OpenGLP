#pragma once
#include <unordered_map>
#include <filesystem>
#include "Region.hpp"
#include "glm/glm.hpp"

struct WorldInfo;

class DiskSystem
{
public:
	static void loadWorld(const std::filesystem::path& path);
	static void loadChunk(const glm::ivec2& position);
	static nbt::NBT getChunk(const glm::ivec2& position);
	static WorldInfo worldInfo;
private:
	static std::filesystem::path worldDir;
	static std::unordered_map<glm::ivec2, nbt::Region> regions;
};