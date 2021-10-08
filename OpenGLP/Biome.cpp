#include "Biome.hpp"
#include <GLM/glm.hpp>
#include "stb_image.hpp"
#include <JSON/json.hpp>
#include <filesystem>
#include <fstream>

std::map<Enums::Biome, Biomes::Biome> Biomes::biomes;

namespace Biomes
{

	void loadBiomes()
	{
		using nlohmann::json;
		namespace fs = std::filesystem;
		fs::path path = "json\\biomes\\";
		json j;
		std::ifstream jsonFile;

		int32_t width;
		int32_t height;
		int32_t bpp;
		unsigned char* grass;
		unsigned char* foliage;
		stbi_set_flip_vertically_on_load(false);
		grass = stbi_load("textures\\colormap\\grass.png", &width, &height, &bpp, 0);
		foliage = stbi_load("textures\\colormap\\foliage.png", &width, &height, &bpp, 0);

		auto getColor = [&](bool downfall, float temp, float down, int& color)
		{
			unsigned short x = static_cast<unsigned short>(255 - (std::clamp(temp, 0.f, 1.f) * 255));
			unsigned short y = static_cast<unsigned short>(255 - (std::clamp(down, 0.f, 1.f) * std::clamp(temp, 0.f, 1.f) * 255));
			if (downfall)
			{
				color = foliage[(y * 256 + x) * 4] << 16 | foliage[(y * 256 + x) * 4 + 1] << 8 | foliage[(y * 256 + x) * 4 + 2];
			}
			else
			{
				color = grass[(y * 256 + x) * 4] << 16 | grass[(y * 256 + x) * 4 + 1] << 8 | grass[(y * 256 + x) * 4 + 2];
			}
		};
		for (const auto& t : fs::directory_iterator(path))
		{
			jsonFile.open(t);
			jsonFile >> j;
			jsonFile.close();
			const Enums::Biome tmpName = Enums::iBiome(t.path().stem().string());
			if (j.contains("temperature"))
				biomes[tmpName].temperature = j["temperature"];
			if (j.contains("downfall"))
				biomes[tmpName].downfall = j["downfall"];
			if (j.contains("category"))
				biomes[tmpName].category = j["category"];
			if (j.contains("precipitation"))
				biomes[tmpName].precipitation = j["precipitation"];
			if (j.contains("effects"))
			{
				json jeffects = j["effects"];
				if (jeffects.contains("sky_color"))
					biomes[tmpName].effects.skyColor = jeffects["sky_color"];
				if (jeffects.contains("fog_color"))
					biomes[tmpName].effects.fogColor = jeffects["fog_color"];
				if (jeffects.contains("water_color"))
					biomes[tmpName].effects.waterColor = jeffects["water_color"];
				if (jeffects.contains("water_fog_color"))
					biomes[tmpName].effects.waterFogColor = jeffects["water_fog_color"];
				if (jeffects.contains("grass_color"))
					biomes[tmpName].effects.grassColor = jeffects["grass_color"];
				else
					getColor(false, biomes[tmpName].temperature, biomes[tmpName].downfall, biomes[tmpName].effects.grassColor);
				if (jeffects.contains("foliage_color"))
					biomes[tmpName].effects.foliageColor = jeffects["foliage_color"];
				else
					getColor(true, biomes[tmpName].temperature, biomes[tmpName].downfall, biomes[tmpName].effects.foliageColor);
			}
		}
		stbi_image_free(grass);
		stbi_image_free(foliage);
	}
}