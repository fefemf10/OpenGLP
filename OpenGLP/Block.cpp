#include "Block.hpp"
#include "CubeHelper.hpp"
#include <string>
std::vector<Blocks::Block> Blocks::blocks(898);
std::vector<Material> Blocks::blockMaterials;
std::unordered_map<std::string, Blocks::BlockMesh> Blocks::blockMesh;
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
		//std::ifstream jsonBlocksFile(paths::assets / paths::minecraft / paths::json / "blocks.bson", std::ios::in | std::ios::binary);
		////std::ofstream jsonBlocksFile1(paths::assets / paths::minecraft / paths::json / "blocks.bson", std::ios::out | std::ios::binary);
		//using nlohmann::json;
		//json j;
		//j = json::from_bson(jsonBlocksFile);
		////jsonBlocksFile >> j;
		//jsonBlocksFile.close();
		//blocks.resize(898);
		//json::to_bson(j, jsonBlocksFile1);
		//jsonBlocksFile.close();
		//for (const auto& [key, value] : j.items())
		//{
		//	Block b;
		//	const size_t index = static_cast<size_t>(Enums::iBlock(key.substr(10)));
		//	/*if (value.contains("properties"))
		//		value["properties"].get_to(b.properties);

		//	b.states.reserve(value["states"].size());
		//	for (auto& [key2, value2] : value["states"].items())
		//	{
		//		State s;
		//		(value2.contains("default")) ? s.defaultState = value2["default"] : s.defaultState = false;
		//		s.id = value2["id"].get<uint16_t>();
		//		if (value2.contains("properties"))
		//			s.properties = value2["properties"].get<std::map<std::string, std::string>>();
		//	}*/
		//	blocks[index] = b;
		//}
	}

	void loadBlockMaterials()
	{
		using nlohmann::json;
		std::ifstream jsonMaterials(paths::assets / paths::minecraft / paths::json / "materials.json", std::ios::in);
		json j;
		jsonMaterials >> j;
		jsonMaterials.close();
		blockMaterials.resize(j.size());
		for (const auto& [key, value] : j.items())
		{
			blockMaterials[static_cast<size_t>(Enums::iMaterial(key.substr(10)))] = value;
		}
		j.clear();
		jsonMaterials.open(paths::assets / paths::minecraft / paths::json / "blocksMaterial.json");
		jsonMaterials >> j;
		jsonMaterials.close();
		for (const auto& [key, value] : j.items())
		{
			blocks[static_cast<size_t>(Enums::iBlock(key.substr(10)))].material = Enums::iMaterial(value.get<std::string>().substr(10));
		}
	}

	void loadBlockStates()
	{
		using nlohmann::json;
		std::ifstream jsonFile;
		blockStates.resize(898);
		for (size_t i = 0; i < blockStates.size(); ++i)
		{
			jsonFile.open(paths::assets / paths::minecraft / paths::blockstates / (Enums::sBlock[i] + std::string(".json")));
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
						std::vector<std::string> out;
						char* next_token{};
						const char* token = strtok_s(const_cast<char*>(s.c_str()), delimiter.data(), &next_token);
						while (token != nullptr)
						{
							out.push_back(std::string(token));
							token = strtok_s(nullptr, delimiter.data(), &next_token);
						}
						return out;
					};

					std::map<Enums::PropertiesBlock, uint8_t> p;
					if (!key.empty())
					{
						std::map<std::string, std::string> properties;
						const std::vector<std::string>& ones = split(key, ",");
						for (size_t i = 0; i < ones.size(); i++)
						{
							const std::vector<std::string>& twos = split(ones[i], "=");
							properties.insert({ twos[0], twos[1] });
						}
						for (const auto& [key, nameProp] : properties)
						{
							const Enums::PropertiesBlock propBlock = Enums::iPropertiesBlock(key);
							uint8_t value{};
							if (static_cast<size_t>(propBlock) > 20)
							{
								if (nameProp == "true")
									value = 1;
								else if (nameProp == "false")
									value = 0;
								else
									value = std::stoi(nameProp);
							}
							else
							{
								switch (propBlock)
								{
								case Enums::PropertiesBlock::ATTACHMENT:
									value = static_cast<uint8_t>(Enums::iAttachment(nameProp));
									break;
								case Enums::PropertiesBlock::AXIS:
									value = static_cast<uint8_t>(Enums::iAxis(nameProp));
									break;
								case Enums::PropertiesBlock::EAST:
									if (nameProp == "true")
										value = 1;
									else if (nameProp == "false")
										value = 0;
									else
										value = static_cast<uint8_t>(Enums::iEast(nameProp));
									break;
								case Enums::PropertiesBlock::FACE:
									value = static_cast<uint8_t>(Enums::iFace(nameProp));
									break;
								case Enums::PropertiesBlock::FACING:
									value = static_cast<uint8_t>(Enums::iFacing(nameProp));
									break;
								case Enums::PropertiesBlock::HALF:
									value = static_cast<uint8_t>(Enums::iHalf(nameProp));
									break;
								case Enums::PropertiesBlock::HINGE:
									value = static_cast<uint8_t>(Enums::iHinge(nameProp));
									break;
								case Enums::PropertiesBlock::INSTRUMENT:
									value = static_cast<uint8_t>(Enums::iInstrument(nameProp));
									break;
								case Enums::PropertiesBlock::LEAVES:
									value = static_cast<uint8_t>(Enums::iLeaves(nameProp));
									break;
								case Enums::PropertiesBlock::NORTH:
									if (nameProp == "true")
										value = 1;
									else if (nameProp == "false")
										value = 0;
									else
										value = static_cast<uint8_t>(Enums::iNorth(nameProp));
									break;
								case Enums::PropertiesBlock::MODE:
									value = static_cast<uint8_t>(Enums::iMode(nameProp));
									break;
								case Enums::PropertiesBlock::ORIENTATION:
									value = static_cast<uint8_t>(Enums::iOrientation(nameProp));
									break;
								case Enums::PropertiesBlock::PART:
									value = static_cast<uint8_t>(Enums::iPart(nameProp));
									break;
								case Enums::PropertiesBlock::SCULK_SENSOR_PHASE:
									value = static_cast<uint8_t>(Enums::iSculkSensorPhase(nameProp));
									break;
								case Enums::PropertiesBlock::SHAPE:
									value = static_cast<uint8_t>(Enums::iShape(nameProp));
									break;
								case Enums::PropertiesBlock::SOUTH:
									if (nameProp == "true")
										value = 1;
									else if (nameProp == "false")
										value = 0;
									else
										value = static_cast<uint8_t>(Enums::iSouth(nameProp));
									break;
								case Enums::PropertiesBlock::THICKNESS:
									value = static_cast<uint8_t>(Enums::iThickness(nameProp));
									break;
								case Enums::PropertiesBlock::TILT:
									value = static_cast<uint8_t>(Enums::iTilt(nameProp));
									break;
								case Enums::PropertiesBlock::TYPE:
									value = static_cast<uint8_t>(Enums::iType(nameProp));
									break;
								case Enums::PropertiesBlock::VERTICAL_DIRECTION:
									value = static_cast<uint8_t>(Enums::iVerticalDirection(nameProp));
									break;
								case Enums::PropertiesBlock::WEST:
									if (nameProp == "true")
										value = 1;
									else if (nameProp == "false")
										value = 0;
									else
										value = static_cast<uint8_t>(Enums::iWest(nameProp));
									break;
								default:
									break;
								}
							}
							p.insert({ propBlock, value });
						}
					}
					blockStates[i].variants.insert({ p, v });
				}
			}
			/*else if (j.contains("multipart"))
			{
				for (auto& [key, value] : j["multipart"].items())
				{

				}
			}*/
		}
	}

	void loadModels()
	{
		using nlohmann::json;
		std::ifstream jsonFile;
		TextureAtlas& atlas = RM::getAtlas("blocks");
		for (const auto& t : std::filesystem::directory_iterator(paths::assets / paths::minecraft / paths::models / "block"))
		{
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
			{
				for (auto& [key, value] : j["textures"].get<std::unordered_map<std::string, std::string>>())
				{
					std::string_view tmpValue(value);
					if (tmpValue.starts_with('#'))
					{
						tmpValue.remove_prefix(1);
						models[tmpName].textures.insert({ Enums::iTextureSlot(key), static_cast<GLuint>(Enums::iTextureSlot(tmpValue)) + 65536u });
					}
					else
					{
						if (!value.starts_with("minecraft:"))
							value.insert(0, "minecraft:");
						GLuint id = atlas[value];
						models[tmpName].textures.insert({ Enums::iTextureSlot(key), id });
					}
				}
			}
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
							switch (key[0])
							{
							case 'e':
								d = Enums::Direction::EAST;
								break;
							case 'w':
								d = Enums::Direction::WEST;
								break;
							case 'u':
								d = Enums::Direction::UP;
								break;
							case 'd':
								d = Enums::Direction::DOWN;
								break;
							case 's':
								d = Enums::Direction::SOUTH;
								break;
							case 'n':
								d = Enums::Direction::NORTH;
								break;
							}
							Model::Element::Face face;
							if (value.contains("uv"))
								face.uv = value["uv"].get<glm::vec4>() * glm::vec4{ 0.0625f, 0.0625f, 0.0625f, 0.0625f };
							if (value.contains("texture"))
							{
								std::string_view tmp(value["texture"]);
								tmp.remove_prefix(1);
								face.texture = Enums::iTextureSlot(tmp);
							}
							if (value.contains("cullface"))
							{
								std::string_view cullface(value["cullface"]);
								switch (cullface[0])
								{
								case 'e':
									face.cullface = Enums::Direction::EAST;
									break;
								case 'w':
									face.cullface = Enums::Direction::WEST;
									break;
								case 'u':
									face.cullface = Enums::Direction::UP;
									break;
								case 'd':
									face.cullface = Enums::Direction::DOWN;
									break;
								case 's':
									face.cullface = Enums::Direction::SOUTH;
									break;
								case 'n':
									face.cullface = Enums::Direction::NORTH;
									break;
								}
							}
							if (value.contains("rotation"))
								face.rotation = value["rotation"];
							if (value.contains("tintindex"))
								face.tintindex = 0;
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
					if (value2 > 65536)
						value2 = models[key].textures[static_cast<Enums::TextureSlot>(value2 - 65536)];
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
	void generateBlockMesh()
	{
		/*std::array<glm::vec3, 8> cube;
		for (const auto& [key, value] : Blocks::models)
		{
			std::vector<glm::vec3>& vertex = blockMesh[key].vertex;
			std::vector<glm::vec3>& UV = blockMesh[key].UV;
			std::vector<uint32_t>& indicies = blockMesh[key].indicies;
			uint32_t countVertex{};
			for (const auto& item : value.elements)
			{
				cube = {
					glm::vec3{ item.from.x, item.to.y, item.to.z },
					{ item.from.x, item.from.y, item.to.z },
					{ item.to.x, item.from.y, item.to.z },
					{ item.to.x, item.to.y, item.to.z },
					{ item.from.x, item.to.y, item.from.z },
					{ item.from.x, item.from.y, item.from.z },
					{ item.to.x, item.from.y, item.from.z },
					{ item.to.x, item.to.y, item.from.z }
				};
				if (item.rotation.rescale)
				{
					if (glm::abs(item.rotation.angle) == 45.f)
					{
						const glm::vec3 rescale(1.41421358f, 1.f, 1.41421358f);
						const glm::vec3 origin(0.5f, 0.f, 0.5f);
						cube[0] -= origin;
						cube[1] -= origin;
						cube[2] -= origin;
						cube[3] -= origin;
						cube[4] -= origin;
						cube[5] -= origin;
						cube[6] -= origin;
						cube[7] -= origin;
						cube[0] *= rescale;
						cube[1] *= rescale;
						cube[2] *= rescale;
						cube[3] *= rescale;
						cube[4] *= rescale;
						cube[5] *= rescale;
						cube[6] *= rescale;
						cube[7] *= rescale;
						cube[0] += origin;
						cube[1] += origin;
						cube[2] += origin;
						cube[3] += origin;
						cube[4] += origin;
						cube[5] += origin;
						cube[6] += origin;
						cube[7] += origin;
					}
					else
					{
						const glm::vec3 rescale(1.08239223f, 1.f, 1.08239223f);
						const glm::vec3 origin(0.5f, 0.f, 0.5f);
						cube[0] -= origin;
						cube[1] -= origin;
						cube[2] -= origin;
						cube[3] -= origin;
						cube[4] -= origin;
						cube[5] -= origin;
						cube[6] -= origin;
						cube[7] -= origin;
						cube[0] *= rescale;
						cube[1] *= rescale;
						cube[2] *= rescale;
						cube[3] *= rescale;
						cube[4] *= rescale;
						cube[5] *= rescale;
						cube[6] *= rescale;
						cube[7] *= rescale;
						cube[0] += origin;
						cube[1] += origin;
						cube[2] += origin;
						cube[3] += origin;
						cube[4] += origin;
						cube[5] += origin;
						cube[6] += origin;
						cube[7] += origin;
					}
				}
				if (item.rotation.angle != 0.f)
					CubeHelper::rotateCube(cube, item.rotation.axis, -item.rotation.angle, item.rotation.origin);
				std::array<std::array<glm::vec3, 4>, 6> cubefaces;
				cubefaces =
				{
					std::array<glm::vec3, 4>{ cube[3], cube[2], cube[6], cube[7] },
					{ cube[4], cube[5], cube[1], cube[0] },
					{ cube[4], cube[0], cube[3], cube[7] },
					{ cube[1], cube[5], cube[6], cube[2] },
					{ cube[0], cube[1], cube[2], cube[3] },
					{ cube[7], cube[6], cube[5], cube[4] }
				};
				vertex.insert(vertex.end(), cubefaces.begin(), cubefaces.end());
				rotateFunc(f.uv, f.rotation, texture);
				indicies.insert(indicies.end(), { countVertex, countVertex + 1, countVertex + 3, countVertex + 1, countVertex + 2, countVertex + 3 });
				countVertex += 4;
			}
		}*/
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