#include "Section.hpp"
#include "World.hpp"
PaletteItem Section::stone{ .id = Enums::Block::STONE };

Section::Section(World& world) : world(world)
{
}

Section::Section(const Section& other) noexcept : world(other.world)
{
}

glm::i8vec3 Section::validPos(const glm::i8vec3& pos) noexcept
{
	return glm::i8vec3((pos.x < 0) * 15 + (pos.x >= 0 && pos.x <= 15) * pos.x, (pos.y < 0) * 15 + (pos.y >= 0 && pos.y <= 15) * pos.y, (pos.z < 0) * 15 + (pos.z >= 0 && pos.z <= 15) * pos.z);
}
glm::ivec3 Section::validSectionPos(const glm::i8vec3& pos) noexcept
{
	return glm::ivec3(-(pos.x < 0) + (pos.x > 15), -(pos.y < 0) + (pos.y > 15), -(pos.z < 0) + (pos.z > 15));
}
bool Section::hasOnlyAir() const
{
	return !(!blockPalette.empty() && !(blockPalette.size() == 1 && blockPalette[0].id == Enums::Block::AIR));
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