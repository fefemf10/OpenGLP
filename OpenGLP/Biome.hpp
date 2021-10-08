#pragma once
#include <map>
#include "Enums.hpp"


namespace Biomes
{
	struct Biome
	{
		struct Effects
		{
			int fogColor;
			int foliageColor;
			int grassColor;
			int skyColor;
			int waterColor;
			int waterFogColor;
		};
		Effects effects{};
		float temperature;
		float downfall;
		float depth;
		float scale;
		std::string category;
		std::string precipitation;
		uint8_t id;
	};
	void loadBiomes();

	extern std::map<Enums::Biome, Biome> biomes;
}