#include "Block.hpp"

std::vector<Blocks::Block> Blocks::blocks;
std::vector<Material> Blocks::blockMaterials;
std::vector<Blocks::BlockStates> Blocks::blockStates;
std::unordered_map<std::string, Blocks::Model> Blocks::models;
namespace Blocks
{
	void to_json(nlohmann::json& j, const Variant& variant)
	{
		j["model"] = variant.model;
		j["x"] = variant.x;
		j["y"] = variant.y;
		j["uvlock"] = variant.uvlock;
		j["weight"] = variant.weight;
	}
	void from_json(const nlohmann::json& j, Variant& variant)
	{
		variant.model = j["model"];
		if (j.contains("x"))
			variant.x = j["x"];
		if (j.contains("y"))
			variant.y = j["y"];
		if (j.contains("uvlock"))
			variant.uvlock = j["uvlock"];
		if (j.contains("weight"))
			variant.weight = j["weight"];
	}
	void loadBlocks()
	{
		std::ifstream jsonBlocksFile(paths::assets / paths::minecraft / paths::json / "blocks.json", std::ios::in);
		using nlohmann::json;
		json j;
		jsonBlocksFile >> j;
		jsonBlocksFile.close();
		blocks.resize(j.size());
		for (auto& [key, value] : j.items())
		{
			Block b;
			std::string blockName = key.substr(10);
			const size_t index = static_cast<size_t>(Enums::iBlock(blockName));
			/*if (value.contains("properties"))
				value["properties"].get_to(b.properties);

			b.states.reserve(value["states"].size());
			for (auto& [key2, value2] : value["states"].items())
			{
				State s;
				(value2.contains("default")) ? s.defaultState = value2["default"] : s.defaultState = false;
				s.id = value2["id"].get<uint16_t>();
				if (value2.contains("properties"))
					s.properties = value2["properties"].get<std::map<std::string, std::string>>();
			}*/
			blocks[index] = b;
		}
	}

	void loadBlockMaterials()
	{
		using nlohmann::json;
		std::ifstream jsonMaterials(paths::assets / paths::minecraft / paths::json / "materials.json", std::ios::in);
		json j;
		jsonMaterials >> j;
		jsonMaterials.close();
		blockMaterials.resize(j.size());
		for (auto& [key, value] : j.items())
		{
			std::string nameMaterial = key.substr(10);
			blockMaterials[static_cast<size_t>(Enums::iMaterial(nameMaterial))] = value;
		}
		j.clear();
		jsonMaterials.open(paths::assets / paths::minecraft / paths::json / "blocksMaterial.json");
		jsonMaterials >> j;
		jsonMaterials.close();
		for (auto& [key, value] : j.items())
		{
			std::string nameMaterial = value.get<std::string>().substr(10);
			std::string nameBlock = key.substr(10);
			blocks[static_cast<size_t>(Enums::iBlock(nameBlock))].material = Enums::iMaterial(nameMaterial);
		}
	}

	void loadBlockStates()
	{
		using nlohmann::json;
		const std::string name = "minecraft:block/";
		std::ifstream jsonFile;
		blockStates.resize(898);
		for (size_t i = 0; i < blockStates.size(); ++i)
		{
			std::string tmpName = Enums::sBlock[i];
			jsonFile.open(paths::assets / paths::minecraft / paths::blockstates /  (tmpName + ".json"));
			json j;
			jsonFile >> j;
			jsonFile.close();

			if (j.contains("variants"))
			{
				for (auto& [key, value] : j["variants"].items())
				{
					std::vector<Variant> v;
					if (value.is_array())
					{
						v = value.get<std::vector<Variant>>();
					}
					else
					{
						v.push_back(value.get<Variant>());
					}
					auto split = [](const std::string& s, const std::string& delimiter) -> std::vector<std::string>
					{
						size_t pos_start = 0, pos_end, delim_len = delimiter.length();
						std::string token;
						std::vector<std::string> res;
						while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
						{
							token = s.substr(pos_start, pos_end - pos_start);
							pos_start = pos_end + delim_len;
							res.push_back(token);
						}
						res.push_back(s.substr(pos_start));
						return res;
					};

					std::map<Enums::PropertiesBlock, uint8_t> p;
					if (!key.empty())
					{
						std::map<std::string, std::string> properties;
						const std::vector<std::string> ones = split(key, ",");
						for (size_t i = 0; i < ones.size(); i++)
						{
							const std::vector<std::string> twos = split(ones[i], "=");
							properties.insert({ twos[0], twos[1] });
						}
						for (auto& [key, value] : properties)
						{
							std::string nameProp = key;
							Enums::PropertiesBlock propBlock = Enums::iPropertiesBlock(nameProp);
							nameProp = value;
							if (static_cast<size_t>(propBlock) > 20)
							{
								if (nameProp == "true")
									p.insert({ propBlock, 1 });
								else if (nameProp == "false")
									p.insert({ propBlock, 0 });
								else
									p.insert({ propBlock, std::stoi(nameProp) });
							}
							else
							{
								if (propBlock == Enums::PropertiesBlock::ATTACHMENT)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iAttachment(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::AXIS)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iAxis(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::EAST)
								{
									if (nameProp == "true")
										p.insert({ propBlock, 1 });
									else if (nameProp == "false")
										p.insert({ propBlock, 0 });
									else
										p.insert({ propBlock, static_cast<uint8_t>(Enums::iEast(nameProp)) });
								}
								else if (propBlock == Enums::PropertiesBlock::FACE)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iFace(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::FACING)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iFacing(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::HALF)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iHalf(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::HINGE)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iHinge(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::INSTRUMENT)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iInstrument(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::LEAVES)
									p.insert({ propBlock,static_cast<uint8_t>(Enums::iLeaves(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::NORTH)
								{
									if (nameProp == "true")
										p.insert({ propBlock, 1 });
									else if (nameProp == "false")
										p.insert({ propBlock, 0 });
									else
										p.insert({ propBlock, static_cast<uint8_t>(Enums::iNorth(nameProp)) });
								}
								else if (propBlock == Enums::PropertiesBlock::MODE)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iMode(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::ORIENTATION)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iOrientation(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::PART)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iPart(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::SCULK_SENSOR_PHASE)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iSculkSensorPhase(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::SHAPE)
									p.insert({ propBlock,static_cast<uint8_t>(Enums::iShape(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::SOUTH)
								{
									if (nameProp == "true")
										p.insert({ propBlock, 1 });
									else if (nameProp == "false")
										p.insert({ propBlock, 0 });
									else
										p.insert({ propBlock, static_cast<uint8_t>(Enums::iSouth(nameProp)) });
								}
								else if (propBlock == Enums::PropertiesBlock::TYPE)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iType(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::THICKNESS)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iThickness(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::TILT)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iTilt(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::VERTICAL_DIRECTION)
									p.insert({ propBlock, static_cast<uint8_t>(Enums::iVerticalDirection(nameProp)) });
								else if (propBlock == Enums::PropertiesBlock::WEST)
								{
									if (nameProp == "true")
										p.insert({ propBlock, 1 });
									else if (nameProp == "false")
										p.insert({ propBlock, 0 });
									else
										p.insert({ propBlock, static_cast<uint8_t>(Enums::iWest(nameProp)) });
								}
							}
						}
					}
					blockStates[i].variants.insert({ p, v });
				}
			}
			else if (j.contains("multipart"))
			{
				for (auto& [key, value] : j["multipart"].items())
				{

				}
			}
		}
	}

	void loadModels()
	{
		using nlohmann::json;
		std::ifstream jsonFile;
		for (const auto& t : std::filesystem::directory_iterator(paths::assets / paths::minecraft / paths::models / "block"))
		{
			bool flag{};
			const std::string tmpName = "minecraft:block/" + t.path().stem().string();
			jsonFile.open(t);
			json j;
			jsonFile >> j;
			jsonFile.close();
			if (j.contains("parent"))
				models[tmpName].parent = j["parent"];
			/*if (j.contains("display"))
			{
				for (auto& [key, value] : j["display"].items())
				{
					Model::Position pos;
					if (value.contains("rotation"))
					{
						std::vector<double> rot = value["rotation"];
						pos.rotation.x = rot[0];
						pos.rotation.y = rot[1];
						pos.rotation.z = rot[2];
					}
					if (value.contains("translation"))
					{
						std::vector<double> trans = value["translation"];
						pos.translation.x = trans[0];
						pos.translation.y = trans[1];
						pos.translation.z = trans[2];
					}
					if (value.contains("scale"))
					{
						std::vector<double> scale = value["scale"];
						pos.scale.x = scale[0];
						pos.scale.y = scale[1];
						pos.scale.z = scale[2];
					}
					models[tmpName].display.insert({ key, pos });
				}
			}*/
			if (j.contains("textures"))
				models[tmpName].textures = j["textures"].get<std::unordered_map<std::string, std::string>>();
			if (j.contains("elements"))
			{
				for (auto& item : j["elements"])
				{
					Model::Element e;
					if (item.contains("from"))
						e.from = item["from"].get<glm::vec3>() * glm::vec3{ 0.0625f, 0.0625f, 0.0625f };
					if (item.contains("to"))
						e.to = item["to"].get<glm::vec3>() * glm::vec3{ 0.0625f, 0.0625f, 0.0625f };
					if (item.contains("rotation"))
					{
						if (item["rotation"].contains("origin"))
							e.rotation.origin = item["rotation"]["origin"].get<glm::vec3>() * glm::vec3{ 0.0625f, 0.0625f, 0.0625f };
						if (item["rotation"].contains("axis"))
						{
							const std::string axis = item["rotation"]["axis"];
							if (axis == "x")
								e.rotation.axis = Enums::Axis::X;
							else if (axis == "y")
								e.rotation.axis = Enums::Axis::Y;
							else
								e.rotation.axis = Enums::Axis::Z;
						}
						if (item["rotation"].contains("angle"))
							e.rotation.angle = item["rotation"]["angle"];
						if (item["rotation"].contains("rescale"))
							e.rotation.rescale = item["rotation"]["rescale"];
					}
					if (item.contains("faces"))
					{
						for (auto& [key, value] : item["faces"].items())
						{
							Enums::Direction d;
							if (key.starts_with('e'))
								d = Enums::Direction::EAST;
							else if (key.starts_with('w'))
								d = Enums::Direction::WEST;
							else if (key.starts_with('u'))
								d = Enums::Direction::UP;
							else if (key.starts_with('d'))
								d = Enums::Direction::DOWN;
							else if (key.starts_with('s'))
								d = Enums::Direction::SOUTH;
							else if (key.starts_with('n'))
								d = Enums::Direction::NORTH;
							Model::Element::Face face;
							if (value.contains("uv"))
								face.uv = value["uv"].get<glm::vec4>() * glm::vec4{ 0.0625f, 0.0625f, 0.0625f, 0.0625f };
							if (value.contains("texture"))
								face.texture = value["texture"];
							if (value.contains("cullface"))
							{
								 const std::string cullface = value["cullface"];
								 if (cullface.starts_with('e'))
									 face.cullface = Enums::Direction::EAST;
								 else if (cullface.starts_with('w'))
									 face.cullface = Enums::Direction::WEST;
								 else if (cullface.starts_with('u'))
									 face.cullface = Enums::Direction::UP;
								 else if (cullface.starts_with('d'))
									 face.cullface = Enums::Direction::DOWN;
								 else if (cullface.starts_with('s'))
									 face.cullface = Enums::Direction::SOUTH;
								 else if (cullface.starts_with('n'))
									 face.cullface = Enums::Direction::NORTH;
							}
							if (value.contains("rotation"))
								face.rotation = value["rotation"];
							if (value.contains("tintindex"))
								face.tintindex = value["tintindex"];
							e.faces.insert({ d, face });
						}
					}
					models[tmpName].elements.push_back(e);
				}
			}
		}
		auto a = [](std::string str) { if (!str.starts_with("minecraft:")) return str.insert(0, "minecraft:"); else return str; };
		for (auto& [key, value] : models)
		{
			if (key == "minecraft:block/block" || key == "minecraft:block/cube" || key == "minecraft:block/cube_all" || key == "minecraft:block/cube_bottom_top" ||
				key == "minecraft:block/cube_column" || key == "minecraft:block/cube_column_horizontal" || key == "minecraft:block/cube_column_mirrored" || key == "minecraft:block/cube_directional" ||
				key == "minecraft:block/cube_mirrored" || key == "minecraft:block/cube_mirrored_all" || key == "minecraft:block/cube_top")
				continue;
			std::string parentKey = key;
			while (models.contains(parentKey))
			{
				const Model& parent = models[parentKey];
				if (models[key].elements.empty())
					models[key].elements.insert(models[key].elements.cbegin(), parent.elements.cbegin(), parent.elements.cend());
				models[key].textures.insert(parent.textures.cbegin(), parent.textures.cend());
				for (auto& [key2, value2] : models[key].textures)
				{
					if (value2.starts_with("#"))
						value2 = models[key].textures[value2.substr(1)];
				}
				if (parent.parent.has_value())
					parentKey = a(parent.parent.value());
				else
					break;
			}
		}
	}

	void loadTextures()
	{
		RM::loadAtlas("blocks", TextureAtlas(1, { 16, 16, 1536 }, GL_RGBA8));
		TextureAtlas& atlas = RM::getAtlas("blocks");
		const std::string name = "minecraft:block/";
		for (const auto& t : std::filesystem::directory_iterator(paths::assets / paths::minecraft / paths::textures / "block"))
		{
			if (t.path().extension() != ".mcmeta")
				atlas.loadTexture(name + t.path().stem().string(), t.path().string());
		}
	}
	const Variant& BlockStates::getVariant(std::map<Enums::PropertiesBlock, uint8_t> prop)
	{
		//std::random_device rd;
		//std::mt19937 gen(rd());
		std::erase_if(prop, [&](const std::pair<Enums::PropertiesBlock, uint8_t>& item) { return !(*this->variants.begin()).first.contains(item.first);  });
		return this->variants[prop][0];
		//if (variants.contains(prop))
		//{
		//	//const std::vector<Variant>& variants = this->variants[prop].first;
		//	//std::uniform_int_distribution<uint32_t> d(0u, variants.size() - 1u);
		//	//return variants[0];
		//	
		//}
		//else
		//{
		//	//const std::vector<Variant>& variants = (*this->variants.begin()).second.first;
		//	//std::uniform_int_distribution<uint32_t> d(0u, variants.size() - 1u);
		//	//return variants[0];
		//	return (*this->variants.begin()).second[0];
		//}
	}
}