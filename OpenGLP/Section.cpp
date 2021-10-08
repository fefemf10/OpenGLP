#include "Section.hpp"
PaletteItem Section::stone{ .id = Enums::Block::STONE };

Section::Section()
{
	vao.addVBO(3);
	vaoTransperent.addVBO(3);
}

const PaletteItem& Section::getBlock(const glm::i8vec3& blockPos)
{
	if (blockPos.y > 15)
	{
		return neighbors[5] ? neighbors[5]->getBlock({ blockPos.x, 0, blockPos.z }) : Section::stone;
	}
	else if (blockPos.y < 0)
	{
		return neighbors[0] ? neighbors[0]->getBlock({ blockPos.x, 15, blockPos.z }) : Section::stone;
	}
	else if (blockPos.x > 15)
	{
		return neighbors[3] ? neighbors[3]->getBlock({ 0, blockPos.y, blockPos.z }) : Section::stone;
	}
	else if (blockPos.x < 0)
	{
		return neighbors[2] ? neighbors[2]->getBlock({ 15, blockPos.y, blockPos.z }) : Section::stone;
	}
	else if (blockPos.z > 15)
	{
		return neighbors[4] ? neighbors[4]->getBlock({ blockPos.x, blockPos.y, 0 }) : Section::stone;
	}
	else if (blockPos.z < 0)
	{
		return neighbors[1] ? neighbors[1]->getBlock({ blockPos.x, blockPos.y, 15 }) : Section::stone;
	}
	else
	{
		if (blockPalette.empty() || dataBlock.empty())
			return Section::stone;
		const int32_t index = (((blockPos.y << 4u) + blockPos.z) << 4u) + blockPos.x;
		const uint16_t startLong = index / countIndexPerLongBlock;
		const uint8_t startBit = (index % countIndexPerLongBlock) * bitsPerBlock;
		const int64_t id{ dataBlock[startLong] >> startBit };
		return blockPalette[id & maskBlock];
	}
}

void Section::gen(uint64_t seed, const glm::vec2& chunkPos)
{
}

void Section::genMesh(const glm::ivec2& position)
{
	TextureAtlas& atlas = RM::getAtlas("blocks");
	auto a = [](std::string str) { if (!str.starts_with("minecraft:")) return str.insert(0, "minecraft:"); else return str; };
	auto rotateFunc = [](const glm::vec4& uv, int angle, std::vector<glm::vec3>& UV, GLuint texture)
	{
		switch (angle)
		{
		case 90:
			UV.push_back({ uv.z, uv.y, texture });
			UV.push_back({ uv.x, uv.y, texture });
			UV.push_back({ uv.x, uv.w, texture });
			UV.push_back({ uv.z, uv.w, texture });
			break;
		case 180:
			UV.push_back({ uv.z, uv.w, texture });
			UV.push_back({ uv.z, uv.y, texture });
			UV.push_back({ uv.x, uv.y, texture });
			UV.push_back({ uv.x, uv.w, texture });
			break;
		case 270:
			UV.push_back({ uv.x, uv.w, texture });
			UV.push_back({ uv.z, uv.w, texture });
			UV.push_back({ uv.z, uv.y, texture });
			UV.push_back({ uv.x, uv.y, texture });
			break;
		default:
			UV.push_back({ uv.x, uv.y, texture });
			UV.push_back({ uv.x, uv.w, texture });
			UV.push_back({ uv.z, uv.w, texture });
			UV.push_back({ uv.z, uv.y, texture });
			break;
		}

	};
	auto rotateCube = [](std::array<glm::vec3, 8>& cube, const Enums::Axis& axis, const float& angle, const glm::vec3& origin)
	{
		glm::mat4 rot(1.f);
		cube[0] -= origin;
		cube[1] -= origin;
		cube[2] -= origin;
		cube[3] -= origin;
		cube[4] -= origin;
		cube[5] -= origin;
		cube[6] -= origin;
		cube[7] -= origin;
		if (axis == Enums::Axis::X)
			rot = glm::rotate(rot, glm::radians(angle), glm::vec3(1.f, 0.f, 0.f));
		else if (axis == Enums::Axis::Y)
			rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
		else if (axis == Enums::Axis::Z)
			rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
		cube[0] = static_cast<glm::vec3>(glm::vec4(cube[0], 1.f) * rot);
		cube[1] = static_cast<glm::vec3>(glm::vec4(cube[1], 1.f) * rot);
		cube[2] = static_cast<glm::vec3>(glm::vec4(cube[2], 1.f) * rot);
		cube[3] = static_cast<glm::vec3>(glm::vec4(cube[3], 1.f) * rot);
		cube[4] = static_cast<glm::vec3>(glm::vec4(cube[4], 1.f) * rot);
		cube[5] = static_cast<glm::vec3>(glm::vec4(cube[5], 1.f) * rot);
		cube[6] = static_cast<glm::vec3>(glm::vec4(cube[6], 1.f) * rot);
		cube[7] = static_cast<glm::vec3>(glm::vec4(cube[7], 1.f) * rot);
		cube[0] += origin;
		cube[1] += origin;
		cube[2] += origin;
		cube[3] += origin;
		cube[4] += origin;
		cube[5] += origin;
		cube[6] += origin;
		cube[7] += origin;
	};
	auto vertexPush = [](std::vector<glm::vec3>& vertex, const std::array<glm::vec3, 4>& quad)
	{
		vertex.insert(vertex.end(), quad.begin(), quad.end());
	};
	auto indiciesPush = [](std::vector<uint32_t>& indicies, uint32_t& countVertex)
	{
		indicies.push_back(countVertex);
		indicies.push_back(countVertex + 1);
		indicies.push_back(countVertex + 3);
		indicies.push_back(countVertex + 1);
		indicies.push_back(countVertex + 2);
		indicies.push_back(countVertex + 3);
		countVertex += 4;
	};
	auto addQuad = [&](Enums::Direction d, const Blocks::Model::Element::Face& f, const std::array<std::array<glm::vec3, 4>, 6>& cube, const std::map<Enums::Direction, Blocks::Model::Element::Face>& ifaces,
		const std::unordered_map<std::string, std::string>& textures, const glm::vec3& colorFloat,
		std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& UV, std::vector<glm::vec3>& color, std::vector<uint32_t>& indicies, uint32_t& countVertex)
	{
		vertexPush(vertex, cube[static_cast<size_t>(std::log2f(static_cast<float>(d)))]);
		if (f.tintindex)
		{
			color.push_back(colorFloat);
			color.push_back(colorFloat);
			color.push_back(colorFloat);
			color.push_back(colorFloat);
		}
		else
		{
			color.push_back({ 1.f,1.f,1.f });
			color.push_back({ 1.f,1.f,1.f });
			color.push_back({ 1.f,1.f,1.f });
			color.push_back({ 1.f,1.f,1.f });
		}
		const unsigned int texture = atlas[a(textures.at(f.texture.substr(1)))];
		rotateFunc(f.uv, f.rotation, UV, texture);
		indiciesPush(indicies, countVertex);
	};
	auto opacity = [](const Enums::Block& block) -> bool
	{
		switch (block)
		{
		case Enums::Block::GLASS:
		case Enums::Block::GLASS_PANE:
		case Enums::Block::WHITE_STAINED_GLASS:
		case Enums::Block::ORANGE_STAINED_GLASS:
		case Enums::Block::MAGENTA_STAINED_GLASS:
		case Enums::Block::LIGHT_BLUE_STAINED_GLASS:
		case Enums::Block::YELLOW_STAINED_GLASS:
		case Enums::Block::LIME_STAINED_GLASS:
		case Enums::Block::PINK_STAINED_GLASS:
		case Enums::Block::GRAY_STAINED_GLASS:
		case Enums::Block::LIGHT_GRAY_STAINED_GLASS:
		case Enums::Block::CYAN_STAINED_GLASS:
		case Enums::Block::PURPLE_STAINED_GLASS:
		case Enums::Block::BLUE_STAINED_GLASS:
		case Enums::Block::BROWN_STAINED_GLASS:
		case Enums::Block::GREEN_STAINED_GLASS:
		case Enums::Block::RED_STAINED_GLASS:
		case Enums::Block::BLACK_STAINED_GLASS:
		case Enums::Block::WHITE_STAINED_GLASS_PANE:
		case Enums::Block::ORANGE_STAINED_GLASS_PANE:
		case Enums::Block::MAGENTA_STAINED_GLASS_PANE:
		case Enums::Block::LIGHT_BLUE_STAINED_GLASS_PANE:
		case Enums::Block::YELLOW_STAINED_GLASS_PANE:
		case Enums::Block::LIME_STAINED_GLASS_PANE:
		case Enums::Block::PINK_STAINED_GLASS_PANE:
		case Enums::Block::GRAY_STAINED_GLASS_PANE:
		case Enums::Block::LIGHT_GRAY_STAINED_GLASS_PANE:
		case Enums::Block::CYAN_STAINED_GLASS_PANE:
		case Enums::Block::PURPLE_STAINED_GLASS_PANE:
		case Enums::Block::BLUE_STAINED_GLASS_PANE:
		case Enums::Block::BROWN_STAINED_GLASS_PANE:
		case Enums::Block::GREEN_STAINED_GLASS_PANE:
		case Enums::Block::RED_STAINED_GLASS_PANE:
		case Enums::Block::BLACK_STAINED_GLASS_PANE:
		case Enums::Block::TINTED_GLASS:
		case Enums::Block::WATER:
		case Enums::Block::ICE:
		case Enums::Block::FROSTED_ICE:
			return true;
		default:
			return false;
		}
	};
	countVertex = 0;
	countVertexTransperent = 0;
	modified = false;
	for (int8_t y = 0; y < 16; ++y)
	{
		for (int8_t z = 0; z < 16; ++z)
		{
			for (int8_t x = 0; x < 16; ++x)
			{
				const PaletteItem& pitem = getBlock({ x, y, z });
				const Blocks::Block& block = Blocks::blocks[static_cast<size_t>(pitem.id)];
				if (block.material != Enums::Material::AIR)
				{
					uint8_t typeColor{};
					if (pitem.id == Enums::Block::GRASS_BLOCK || pitem.id == Enums::Block::GRASS || pitem.id == Enums::Block::TALL_GRASS || pitem.id == Enums::Block::FERN)
						typeColor = 1;
					else if (block.material == Enums::Material::LEAVES)
						typeColor = 2;
					const glm::vec3 positionBlock(x + (position.x << 4), height * 16 + y, z + (position.y << 4));
					if (!Blocks::blockStates[static_cast<size_t>(pitem.id)].variants.empty())
					{
						const Blocks::Variant& variant = Blocks::blockStates[static_cast<size_t>(pitem.id)].getVariant(pitem.properties);
						const Blocks::Model& modelBlock = Blocks::models[variant.model];
						const int16_t rotx{ variant.x }, roty{ variant.y };
						for (const Blocks::Model::Element& item : modelBlock.elements)
						{
							std::array<glm::vec3, 8> cube
							{
								glm::vec3{item.from.x, item.to.y, item.to.z},
								{item.from.x, item.from.y, item.to.z},
								{item.to.x, item.from.y, item.to.z},
								{item.to.x, item.to.y, item.to.z},
								{item.from.x, item.to.y, item.from.z},
								{item.from.x, item.from.y, item.from.z},
								{item.to.x, item.from.y, item.from.z},
								{item.to.x, item.to.y, item.from.z}
							};
							if (item.rotation.rescale)
							{
								if (glm::abs(item.rotation.angle) == 45.f)
								{
									const float rescale = 1.41421358f;
									const glm::vec3 origin(0.5f);
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
									const float rescale = 1.08239223f;
									const glm::vec3 origin(0.5f);
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
								rotateCube(cube, item.rotation.axis, -item.rotation.angle, item.rotation.origin);
							if (rotx != 0.f)
								rotateCube(cube, Enums::Axis::X, rotx, glm::vec3(0.5f));
							if (roty != 0.f)
								rotateCube(cube, Enums::Axis::Y, roty, glm::vec3(0.5f));
							cube[0] += positionBlock;
							cube[1] += positionBlock;
							cube[2] += positionBlock;
							cube[3] += positionBlock;
							cube[4] += positionBlock;
							cube[5] += positionBlock;
							cube[6] += positionBlock;
							cube[7] += positionBlock;
							const int32_t indexBiome = ((x / 4) * 4 + (z / 4)) * 4 + y / 4;
							const uint16_t startLongBiome = indexBiome / countIndexPerLongBiome;
							const uint8_t startBitBiome = (indexBiome % countIndexPerLongBiome) * bitsPerBiome;
							const int64_t id{ dataBiome[startLongBiome] >> startBitBiome };
							int hexColor = Biomes::biomes[biomePalette[id & maskBiome]].effects.grassColor;
							const glm::vec3 grassColor{ (hexColor >> 16 & 0xFF) / 255.f, (hexColor >> 8 & 0xFF) / 255.f, (hexColor & 0xFF) / 255.f };
							hexColor = Biomes::biomes[biomePalette[id & maskBiome]].effects.foliageColor;
							const glm::vec3 foliageColor{ (hexColor >> 16 & 0xFF) / 255.f, (hexColor >> 8 & 0xFF) / 255.f, (hexColor & 0xFF) / 255.f };
							glm::vec3 colorFloat = { 1.f, 1.f, 1.f };
							switch (typeColor)
							{
							case 1:
								colorFloat = grassColor;
								break;
							case 2:
								colorFloat = foliageColor;
								break;
							default:
								colorFloat = { 1.f, 1.f, 1.f };
								break;
							}
							const std::array<std::array<glm::vec3, 4>, 6> cubefaces
							{
								std::array<glm::vec3, 4>{ cube[3], cube[2], cube[6], cube[7] },
								{ cube[4], cube[5], cube[1], cube[0] },
								{ cube[4], cube[0], cube[3], cube[7] },
								{ cube[1], cube[5], cube[6], cube[2] },
								{ cube[0], cube[1], cube[2], cube[3] },
								{ cube[7], cube[6], cube[5], cube[4] }
							};

							const std::map<Enums::Direction, Blocks::Model::Element::Face>& blockFaces = item.faces;
							if (blockFaces.contains(Enums::Direction::EAST))
							{
								if (blockFaces.at(Enums::Direction::EAST).cullface == Enums::Direction::EAST)
								{
									const PaletteItem& idxp = getBlock({ x + 1, y, z });
									const Material& idxm = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idxp.id)].material)];
									if (pitem.id != idxp.id && !idxm.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::EAST, blockFaces.at(Enums::Direction::EAST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::EAST, blockFaces.at(Enums::Direction::EAST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::EAST, blockFaces.at(Enums::Direction::EAST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::EAST, blockFaces.at(Enums::Direction::EAST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
							}
							if (blockFaces.contains(Enums::Direction::WEST))
							{
								if (blockFaces.at(Enums::Direction::WEST).cullface == Enums::Direction::WEST)
								{
									const PaletteItem& idxxp = getBlock({ x - 1, y, z });
									const Material& idxxm = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idxxp.id)].material)];
									if (pitem.id != idxxp.id && !idxxm.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::WEST, blockFaces.at(Enums::Direction::WEST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::WEST, blockFaces.at(Enums::Direction::WEST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::WEST, blockFaces.at(Enums::Direction::WEST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::WEST, blockFaces.at(Enums::Direction::WEST), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}

							}
							if (blockFaces.contains(Enums::Direction::UP))
							{
								if (blockFaces.at(Enums::Direction::UP).cullface == Enums::Direction::UP)
								{
									const PaletteItem& idyp = getBlock({ x, y + 1, z });
									const Material& idym = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idyp.id)].material)];
									if (pitem.id != idyp.id && !idym.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::UP, blockFaces.at(Enums::Direction::UP), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::UP, blockFaces.at(Enums::Direction::UP), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::UP, blockFaces.at(Enums::Direction::UP), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::UP, blockFaces.at(Enums::Direction::UP), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
							}

							if (blockFaces.contains(Enums::Direction::DOWN))
							{
								if (blockFaces.at(Enums::Direction::DOWN).cullface == Enums::Direction::DOWN)
								{
									const PaletteItem& idyyp = getBlock({ x, y - 1, z });
									const Material& idyym = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idyyp.id)].material)];
									if (pitem.id != idyyp.id && !idyym.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::DOWN, blockFaces.at(Enums::Direction::DOWN), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::DOWN, blockFaces.at(Enums::Direction::DOWN), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::DOWN, blockFaces.at(Enums::Direction::DOWN), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::DOWN, blockFaces.at(Enums::Direction::DOWN), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
							}
							if (blockFaces.contains(Enums::Direction::SOUTH))
							{
								if (blockFaces.at(Enums::Direction::SOUTH).cullface == Enums::Direction::SOUTH)
								{
									const PaletteItem& idzp = getBlock({ x, y, z + 1 });
									const Material& idzm = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idzp.id)].material)];
									if (pitem.id != idzp.id && !idzm.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::SOUTH, blockFaces.at(Enums::Direction::SOUTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::SOUTH, blockFaces.at(Enums::Direction::SOUTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::SOUTH, blockFaces.at(Enums::Direction::SOUTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::SOUTH, blockFaces.at(Enums::Direction::SOUTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
							}
							if (blockFaces.contains(Enums::Direction::NORTH))
							{
								
								if (blockFaces.at(Enums::Direction::NORTH).cullface == Enums::Direction::NORTH)
								{
									const PaletteItem& idzzp = getBlock({ x, y, z - 1 });
									const Material& idzzm = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(idzzp.id)].material)];
									if (pitem.id != idzzp.id && !idzzm.solidBlocking)
										if (!opacity(pitem.id))
											addQuad(Enums::Direction::NORTH, blockFaces.at(Enums::Direction::NORTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
										else
											addQuad(Enums::Direction::NORTH, blockFaces.at(Enums::Direction::NORTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
								else
								{
									if (!opacity(pitem.id))
										addQuad(Enums::Direction::NORTH, blockFaces.at(Enums::Direction::NORTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertex, UV, color, indicies, countVertex);
									else
										addQuad(Enums::Direction::NORTH, blockFaces.at(Enums::Direction::NORTH), cubefaces, item.faces, modelBlock.textures, colorFloat, vertext, UVt, colort, indiciest, countVertexTransperent);
								}
							}
						}
					}
				}
			}
		}
	}
	if (vertex.empty() && vertext.empty())
		buffer = false;
	else
		buffer = true;
}

void Section::loadBuffer()
{
	if (buffer)
	{
		buffer = false;
		vao.loadData(0, vertex);
		vao.loadData(1, color);
		vao.loadData(2, UV);
		vao.loadIndices(indicies);
		vaoTransperent.loadData(0, vertext);
		vaoTransperent.loadData(1, colort);
		vaoTransperent.loadData(2, UVt);
		vaoTransperent.loadIndices(indiciest);
		vertex.clear();
		color.clear();
		UV.clear();
		indicies.clear();
		vertext.clear();
		colort.clear();
		UVt.clear();
		indiciest.clear();
		visible = true;
	}
}

void Section::draw()
{
	if (buffer)
		loadBuffer();
	if (visible)
		vao.draw();
}

void Section::drawTransperent()
{
	if (buffer)
		loadBuffer();
	if (visible)
		vaoTransperent.draw();
}