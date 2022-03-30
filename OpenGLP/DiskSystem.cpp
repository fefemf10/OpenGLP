#include "DiskSystem.hpp"
#include "NBTFile.hpp"
#include "WorldInfo.hpp"

std::filesystem::path DiskSystem::worldDir;
WorldInfo DiskSystem::worldInfo;
std::unordered_map<glm::ivec2, nbt::Region> DiskSystem::regions;

void DiskSystem::loadWorld(const std::filesystem::path& path)
{
	worldDir = path;
	if (!std::filesystem::exists(std::filesystem::current_path() / "saves"))
		std::filesystem::create_directory(std::filesystem::current_path() / "saves");
	worldDir = std::filesystem::current_path() / "saves" / path;
	if (!std::filesystem::exists(worldDir))
		std::filesystem::create_directory(worldDir);
	nbt::NBTFile level;
	if (std::filesystem::exists(worldDir / "level.dat"))
	{
		level.deserialize((worldDir / "level.dat").string(), true);
	}
	nbt::TagCompound data = level.root.at<nbt::TagCompound>("Data");
	worldInfo.spawnX = data.at<nbt::TagInt>("SpawnX");
	worldInfo.spawnY = data.at<nbt::TagInt>("SpawnY");
	worldInfo.spawnZ = data.at<nbt::TagInt>("SpawnZ");
}

void DiskSystem::loadChunk(const glm::ivec2& position)
{
	const glm::ivec2& regionPos = position >> 5;
	const glm::ivec2 chunkPos(position.x < 0 ? (abs(position.x) % 32 == 0 ? 0 : 32) + position.x % 32 : position.x % 32, position.y < 0 ? (abs(position.y) % 32 == 0 ? 0 : 32) + position.y % 32 : position.y % 32);
	std::fstream region(worldDir / "region" / std::string("r." + std::to_string(regionPos.x) + "." + std::to_string(regionPos.y) + ".mca"), std::ios::in | std::ios::binary);
	if (regions.contains(regionPos))
	{
		if (region.is_open())
		{
			regions[regionPos].deserialize(region, chunkPos.x, chunkPos.y);
		}
	}
	else
	{
		if (region.is_open())
		{
			regions[regionPos].readLT(region);
			regions[regionPos].deserialize(region, chunkPos.x, chunkPos.y);
		}
	}
	region.close();
}

nbt::NBT DiskSystem::getChunk(const glm::ivec2& position)
{
	const glm::ivec2& regionPos = position >> 5;
	const glm::ivec2 chunkPos(position.x < 0 ? (abs(position.x) % 32 == 0 ? 0 : 32) + position.x % 32 : position.x % 32, position.y < 0 ? (abs(position.y) % 32 == 0 ? 0 : 32) + position.y % 32 : position.y % 32);
	return regions[regionPos].chunks[chunkPos.y * 32 + chunkPos.x];
}
