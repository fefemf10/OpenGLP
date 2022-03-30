#include "Section.hpp"
#include "World.hpp"
PaletteItem Section::stone{ .id = Enums::Block::STONE };

Section::Section(World& world) : world(world)
{
}

Section::Section(const Section& other) noexcept : world(other.world)
{
}

const glm::i8vec3& Section::validPos(const glm::i8vec3& pos) noexcept
{
	return glm::i8vec3((pos.x < 0) * 15 + (pos.x >= 0 && pos.x <= 15) * pos.x, (pos.y < 0) * 15 + (pos.y >= 0 && pos.y <= 15) * pos.y, (pos.z < 0) * 15 + (pos.z >= 0 && pos.z <= 15) * pos.z);
}
const glm::ivec3& Section::validSectionPos(const glm::i8vec3& pos) noexcept
{
	return glm::ivec3(-(pos.x < 0) + (pos.x > 15), -(pos.y < 0) + (pos.y > 15), -(pos.z < 0) + (pos.z > 15));
}

bool Section::hasOnlyAir() const
{
	return !(!blockPalette.empty() && !(blockPalette.size() == 1 && blockPalette[0].id == Enums::Block::AIR));
}

void Section::floodfill()
{
	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 6; j++)
		{
			flood[i][j] = false;
		}
	}
	uint8_t bUse[16][16][16]{};
	uint8_t faces{};
	size_t countColor{};
	for (int8_t y = 0; y < 16; ++y)
	{
		for (int8_t z = 0; z < 16; ++z)
		{
			for (int8_t x = 0; x < 16; ++x)
			{
				const PaletteItem& pitem = getBlock({ x, y, z });
				const Blocks::Block& block = Blocks::blocks[static_cast<size_t>(pitem.id)];
				if (block.material == Enums::Material::AIR && !bUse[y][z][x])
				{
					std::queue<glm::ivec3> q;
					q.push({ x, y, z });
					++countColor;
					faces = 0;
					while (!q.empty())
					{
						if (faces == 63)
							break;
						const glm::ivec3 localPos = q.front();
						const glm::ivec3 c[6] =
						{
							localPos + Enums::getVecFacing(Enums::Direction::EAST),
							localPos + Enums::getVecFacing(Enums::Direction::WEST),
							localPos + Enums::getVecFacing(Enums::Direction::UP),
							localPos + Enums::getVecFacing(Enums::Direction::DOWN),
							localPos + Enums::getVecFacing(Enums::Direction::SOUTH),
							localPos + Enums::getVecFacing(Enums::Direction::NORTH),
						};
						const PaletteItem& pie{ getBlock(c[0]) };
						const PaletteItem& piw{ getBlock(c[1]) };
						const PaletteItem& piu{ getBlock(c[2]) };
						const PaletteItem& pid{ getBlock(c[3]) };
						const PaletteItem& pis{ getBlock(c[4]) };
						const PaletteItem& pin{ getBlock(c[5]) };
						const bool bs[6] =
						{
							Blocks::blocks[static_cast<size_t>(pie.id)].material == Enums::Material::AIR,
							Blocks::blocks[static_cast<size_t>(piw.id)].material == Enums::Material::AIR,
							Blocks::blocks[static_cast<size_t>(piu.id)].material == Enums::Material::AIR,
							Blocks::blocks[static_cast<size_t>(pid.id)].material == Enums::Material::AIR,
							Blocks::blocks[static_cast<size_t>(pis.id)].material == Enums::Material::AIR,
							Blocks::blocks[static_cast<size_t>(pin.id)].material == Enums::Material::AIR,
						};
						if (bs[0] && validSectionPos(c[0]) == glm::ivec3(0) && !bUse[c[0].y][c[0].z][c[0].x])
						{
							q.push(c[0]);
							bUse[c[0].y][c[0].z][c[0].x] = countColor;
						}
						if (localPos.x == 0)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::WEST);
						}
						else if (localPos.x == 15)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::EAST);
						}
						if (localPos.y == 0)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::DOWN);
						}
						else if (localPos.y == 15)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::UP);
						}
						if (localPos.z == 0)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::NORTH);
						}
						else if (localPos.z == 15)
						{
							faces |= static_cast<uint8_t>(Enums::Direction::SOUTH);
						}
						if (bs[1] && validSectionPos(c[1]) == glm::ivec3(0) && !bUse[c[1].y][c[1].z][c[1].x])
						{
							q.push(c[1]);
							bUse[c[1].y][c[1].z][c[1].x] = countColor;
						}
						if (bs[2] && validSectionPos(c[2]) == glm::ivec3(0) && !bUse[c[2].y][c[2].z][c[2].x])
						{
							q.push(c[2]);
							bUse[c[2].y][c[2].z][c[2].x] = countColor;
						}
						if (bs[3] && validSectionPos(c[3]) == glm::ivec3(0) && !bUse[c[3].y][c[3].z][c[3].x])
						{
							q.push(c[3]);
							bUse[c[3].y][c[3].z][c[3].x] = countColor;
						}
						if (bs[4] && validSectionPos(c[4]) == glm::ivec3(0) && !bUse[c[4].y][c[4].z][c[4].x])
						{
							q.push(c[4]);
							bUse[c[4].y][c[4].z][c[4].x] = countColor;
						}
						if (bs[5] && validSectionPos(c[5]) == glm::ivec3(0) && !bUse[c[5].y][c[5].z][c[5].x])
						{
							q.push(c[5]);
							bUse[c[5].y][c[5].z][c[5].x] = countColor;
						}
						q.pop();
					}
					const bool bf[6] =
					{
						(faces & static_cast<uint8_t>(Enums::Direction::EAST)) == static_cast<uint8_t>(Enums::Direction::EAST),
						(faces & static_cast<uint8_t>(Enums::Direction::WEST)) == static_cast<uint8_t>(Enums::Direction::WEST),
						(faces & static_cast<uint8_t>(Enums::Direction::UP)) == static_cast<uint8_t>(Enums::Direction::UP),
						(faces & static_cast<uint8_t>(Enums::Direction::DOWN)) == static_cast<uint8_t>(Enums::Direction::DOWN),
						(faces & static_cast<uint8_t>(Enums::Direction::SOUTH)) == static_cast<uint8_t>(Enums::Direction::SOUTH),
						(faces & static_cast<uint8_t>(Enums::Direction::NORTH)) == static_cast<uint8_t>(Enums::Direction::NORTH),
					};
					if (bf[0])
					{
						flood[0][0] |= true;
						flood[0][1] |= bf[1];
						flood[0][2] |= bf[2];
						flood[0][3] |= bf[3];
						flood[0][4] |= bf[4];
						flood[0][5] |= bf[5];
					}
					if (bf[1])
					{
						flood[1][0] |= bf[0];
						flood[1][1] |= true;
						flood[1][2] |= bf[2];
						flood[1][3] |= bf[3];
						flood[1][4] |= bf[4];
						flood[1][5] |= bf[5];
					}
					if (bf[2])
					{
						flood[2][0] |= bf[0];
						flood[2][1] |= bf[1];
						flood[2][2] |= true;
						flood[2][3] |= bf[3];
						flood[2][4] |= bf[4];
						flood[2][5] |= bf[5];
					}
					if (bf[3])
					{
						flood[3][0] |= bf[0];
						flood[3][1] |= bf[1];
						flood[3][2] |= bf[2];
						flood[3][3] |= true;
						flood[3][4] |= bf[4];
						flood[3][5] |= bf[5];
					}
					if (bf[4])
					{
						flood[4][0] |= bf[0];
						flood[4][1] |= bf[1];
						flood[4][2] |= bf[2];
						flood[4][3] |= bf[3];
						flood[4][4] |= true;
						flood[4][5] |= bf[5];
					}
					if (bf[5])
					{
						flood[5][0] |= bf[0];
						flood[5][1] |= bf[1];
						flood[5][2] |= bf[2];
						flood[5][3] |= bf[3];
						flood[5][4] |= bf[4];
						flood[5][5] |= true;
					}
					if (faces == 63)
						return;
				}
			}
		}
	}
}

const PaletteItem& Section::getBlock(const glm::i8vec3& blockPos)
{
	const glm::i8vec3 sectionPos = validSectionPos(blockPos);
	if (sectionPos.x == 0 && sectionPos.y == 0 && sectionPos.z == 0)
	{
		if (blockPalette.empty() || dataBlock.empty())
			return Section::stone;
		const int32_t index = (((blockPos.y << 4u) + blockPos.z) << 4u) + blockPos.x;
		const uint16_t startLong = index / countIndexPerLongBlock;
		const uint8_t startBit = (index % countIndexPerLongBlock) * bitsPerBlock;
		const int64_t id{ dataBlock[startLong] >> startBit };
		return blockPalette[id & maskBlock];
	}
	else
	{
		Section* const sec = world.getSection(this->position + glm::ivec3(sectionPos));
		if (sec && !sec->work.load())
			return sec->getBlock(validPos(blockPos));
		else
			return Section::stone;
	}
}

void Section::gen(uint64_t seed, const glm::vec2& chunkPos)
{
}

void Section::genMesh()
{
	modified.store(false);
	work.store(true);
	TextureAtlas& atlas = RM::getAtlas("blocks");
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
					const int iLight = (y * 16 + z) * 16 + x;
					const float lightFactor = 1.f;//(iLight % 2) ? ((skyLight[iLight / 2]) & 0x0F) / 15.f : ((skyLight[iLight / 2]>>4) & 0x0F) / 15.f;
					uint8_t typeColor{};
					if (pitem.id == Enums::Block::GRASS_BLOCK || pitem.id == Enums::Block::GRASS || pitem.id == Enums::Block::TALL_GRASS || pitem.id == Enums::Block::FERN)
						typeColor = 1;
					else if (block.material == Enums::Material::LEAVES)
						typeColor = 2;
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
					const glm::ivec3 positionBlock(x + (position.x << 4), y + (position.y << 4), z + (position.z << 4));
					if (!Blocks::blockStates[static_cast<size_t>(pitem.id)].variants.empty())
					{
						const Blocks::Variant& variant = Blocks::blockStates[static_cast<size_t>(pitem.id)].getVariant(pitem.properties);
						const Blocks::Model& modelBlock = Blocks::models[variant.model];
						const glm::ivec3 localPos(x, y, z);
						const std::array<std::array<std::array<bool, 3>, 3>, 3> nBlocks =
						{
							std::array<std::array<bool, 3>, 3>
							{
								std::array<bool, 3>
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, -1, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, -1, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, -1, -1)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, -1, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, -1, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, -1, 0)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, -1, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, -1, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, -1, 1)).id)].material)].solid
								}
							},
							{
								std::array<bool, 3>
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 0, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 0, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 0, -1)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 0, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 0, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 0, 0)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 0, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 0, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 0, 1)).id)].material)].solid
								}
							},
							{
								std::array<bool, 3>
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 1, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 1, -1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 1, -1)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 1, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 1, 0)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 1, 0)).id)].material)].solid
								},
								{
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(-1, 1, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(0, 1, 1)).id)].material)].solid,
									Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(getBlock(localPos + glm::ivec3(1, 1, 1)).id)].material)].solid
								}
							}
						};
						const PaletteItem& pie{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::EAST)) };
						const PaletteItem& piw{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::WEST)) };
						const PaletteItem& piu{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::UP)) };
						const PaletteItem& pid{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::DOWN)) };
						const PaletteItem& pis{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::SOUTH)) };
						const PaletteItem& pin{ getBlock(localPos + Enums::getVecFacing(Enums::Direction::NORTH)) };
						const bool ind[6] = {
							nBlocks[1][1][2],
							nBlocks[1][1][0],
							nBlocks[2][1][1],
							nBlocks[0][1][1],
							nBlocks[1][2][1],
							nBlocks[1][0][1],
						};
						for (const Blocks::Model::Element& item : modelBlock.elements)
						{
							if (!opacity(pitem.id))
							{
								CubeHelper cube(*this, nBlocks, positionBlock, colorFloat, localPos, glm::vec2(variant.x, variant.y), item, countVertex, modelBlock.textures);
								cube.addQuad(Enums::Direction::EAST, pitem.id, pie.id, ind[0]);
								cube.addQuad(Enums::Direction::WEST, pitem.id, piw.id, ind[1]);
								cube.addQuad(Enums::Direction::UP, pitem.id, piu.id, ind[2]);
								cube.addQuad(Enums::Direction::DOWN, pitem.id, pid.id, ind[3]);
								cube.addQuad(Enums::Direction::SOUTH, pitem.id, pis.id, ind[4]);
								cube.addQuad(Enums::Direction::NORTH, pitem.id, pin.id, ind[5]);
								cube.add(vertex, UV, color, AO, indicies, countVertex);
							}
							/*else
							{
								CubeHelper cube(*this, pitem, positionBlock, colorFloat, { x, y, z }, glm::vec2(variant.x, variant.y), item, countVertexTransperent, atlas, modelBlock.textures);
								cube.addQuad(Enums::Direction::EAST);
								cube.addQuad(Enums::Direction::WEST);
								cube.addQuad(Enums::Direction::UP);
								cube.addQuad(Enums::Direction::DOWN);
								cube.addQuad(Enums::Direction::SOUTH);
								cube.addQuad(Enums::Direction::NORTH);
								cube.add(vertext, UVt, colort, AOt, indiciest, countVertexTransperent);
							}*/
						}
					}
				}
			}
		}
	}
	buffer.store(true);
	buffert.store(true);
	work.store(false);
}

void Section::loadBuffer()
{
	buffer.store(false);
	buffert.store(false);
	vertex.clear();
	color.clear();
	UV.clear();
	AO.clear();
	indicies.clear();
	/*vertext.clear();
	colort.clear();
	UVt.clear();
	AOt.clear();
	indiciest.clear();*/
}