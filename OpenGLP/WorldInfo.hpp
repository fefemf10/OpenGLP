#pragma once
#include <string>

struct WorldInfo
{
	bool allowCommands;
	long long dayTime;
	int gameType;
	std::string generatorName;
	std::string generatorOptions;
	int generatorVersion;
	bool hardcore;
	bool initialized;
	long long lastPlayed;
	std::string LevelName;
	bool mapFutures;
	bool raining;
	int rainTime;
	long long randomSeed;
	long long sizeOnDisk;
	int spawnX;
	int spawnY;
	int spawnZ;
	bool thundering;
	int thunderTime;
	long long time;
	int version;
};

