#pragma once
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <optional>
#include <GLM/glm.hpp>
#include <JSON/json.hpp>
#include "RM.hpp"
#include "Material.hpp"
#include <GLM/ext/matrix_transform.hpp>
#include "jsonglm.hpp"
#include "Biome.hpp"
#include "Enums.hpp"


namespace Blocks
{
	struct Variant
	{
		std::string model;
		int16_t x{0};
		int16_t y{0};
		uint8_t weight{1};
		bool uvlock{false};
	};
	void to_json(nlohmann::json& j, const Blocks::Variant& variant);
	void from_json(const nlohmann::json& j, Blocks::Variant& variant);
	struct BlockStates
	{
		struct Condition
		{
			std::map<std::map<std::string, std::string>, std::vector<Blocks::Variant>> values;
		};
		std::map<std::map<Enums::PropertiesBlock, uint8_t>, std::vector<Blocks::Variant>> variants;
		std::vector<Condition> multiparts;
		const Variant& getVariant(std::map<Enums::PropertiesBlock, uint8_t> prop);
	};
	struct Model
	{
		struct Element
		{
			struct Rotation
			{
				glm::vec3 origin{8.f, 8.f, 8.f};
				float angle{};
				Enums::Axis axis{};
				bool rescale{};
			};
			struct Face
			{
				std::string texture;
				glm::vec4 uv{ 0.f, 0.f, 1.f, 1.f };
				int32_t rotation{};
				std::optional<int> tintindex;
				Enums::Direction cullface{0};
			};
			Rotation rotation;
			glm::vec3 from{};
			glm::vec3 to{};
			std::map<Enums::Direction, Face> faces;
		};
		struct Position
		{
			glm::dvec3 rotation;
			glm::dvec3 translation;
			glm::dvec3 scale;
		};
		std::optional<std::string> parent;
		//std::unordered_map<std::string, Position> display;
		std::unordered_map<std::string, std::string> textures;
		std::vector<Element> elements;
		bool ambientocclusion{};
	};
	struct Block
	{
		//std::map<std::string, std::vector<std::string>> properties;
		//std::vector<State> states;
		Enums::Material material{};
	};
	void loadBlocks();
	void loadBlockMaterials();
	void loadBlockStates();
	void loadModels();
	void loadTextures();
	extern std::vector<Block> blocks;
	extern std::vector<Material> blockMaterials;
	extern std::vector<Blocks::BlockStates> blockStates;
	extern std::unordered_map<std::string, Blocks::Model> models;
}