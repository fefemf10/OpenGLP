#include "Section.hpp"
#include "World.hpp"
PaletteItem Section::stone{ .id = Enums::Block::STONE };

Section::Section(World& world) : world(world)
{
	vao.addVBO(4);
	vaoTransperent.addVBO(4);
}

Section::Section(const Section& other) noexcept : world(other.world)
{
	vao.addVBO(4);
	vaoTransperent.addVBO(4);
}

const PaletteItem& Section::getBlock(const glm::i8vec3& blockPos)
{
	auto validPos = [](const glm::i8vec3& pos) -> glm::i8vec3
	{
		glm::i8vec3 result(pos);
		if (pos.x < 0)
			result.x = 15;
		else if (pos.x > 15)
			result.x = 0;

		if (pos.y < 0)
			result.y = 15;
		else if (pos.y > 15)
			result.y = 0;

		if (pos.z < 0)
			result.z = 15;
		else if (pos.z > 15)
			result.z = 0;

		return result;
	};
	auto validSectionPos = [](const glm::i8vec3& pos) -> glm::ivec3
	{
		glm::ivec3 result(0);
		if (pos.x < 0)
			result.x = -1;
		else if (pos.x > 15)
			result.x = 1;

		if (pos.y < 0)
			result.y = -1;
		else if (pos.y > 15)
			result.y = 1;

		if (pos.z < 0)
			result.z = -1;
		else if (pos.z > 15)
			result.z = 1;
		return result;
	};
	glm::i8vec3 sectionPos = validSectionPos(blockPos);
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
		Section* sec = world.getSection(this->position + glm::ivec3(sectionPos));
		if (sec)
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
					const int iLight = (y * 16 + z) * 16 + x;
					const float lightFactor = 1.f;//(iLight % 2) ? ((skyLight[iLight / 2]) & 0x0F) / 15.f : ((skyLight[iLight / 2]>>4) & 0x0F) / 15.f;
					uint8_t typeColor{};
					if (pitem.id == Enums::Block::GRASS_BLOCK || pitem.id == Enums::Block::GRASS || pitem.id == Enums::Block::TALL_GRASS || pitem.id == Enums::Block::FERN)
						typeColor = 1;
					else if (block.material == Enums::Material::LEAVES)
						typeColor = 2;
					const glm::ivec3 positionBlock(x + (position.x << 4), y + (position.y << 4), z + (position.z << 4));
					if (!Blocks::blockStates[static_cast<size_t>(pitem.id)].variants.empty())
					{
						const Blocks::Variant& variant = Blocks::blockStates[static_cast<size_t>(pitem.id)].getVariant(pitem.properties);
						const Blocks::Model& modelBlock = Blocks::models[variant.model];
						for (const Blocks::Model::Element& item : modelBlock.elements)
						{
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
							if (!opacity(pitem.id))
							{
								CubeHelper cube(*this, pitem, positionBlock, {x, y, z}, glm::vec2(variant.x, variant.y), item, countVertex, atlas, modelBlock.textures);
								cube.addQuad(Enums::Direction::EAST, colorFloat);
								cube.addQuad(Enums::Direction::WEST, colorFloat);
								cube.addQuad(Enums::Direction::UP, colorFloat);
								cube.addQuad(Enums::Direction::DOWN, colorFloat);
								cube.addQuad(Enums::Direction::SOUTH, colorFloat);
								cube.addQuad(Enums::Direction::NORTH, colorFloat);
								cube.add(vertex, UV, color, AO, indicies, countVertex);
							}
							else
							{
								CubeHelper cube(*this, pitem, positionBlock, { x, y, z }, glm::vec2(variant.x, variant.y), item, countVertexTransperent, atlas, modelBlock.textures);
								cube.addQuad(Enums::Direction::EAST, colorFloat);
								cube.addQuad(Enums::Direction::WEST, colorFloat);
								cube.addQuad(Enums::Direction::UP, colorFloat);
								cube.addQuad(Enums::Direction::DOWN, colorFloat);
								cube.addQuad(Enums::Direction::SOUTH, colorFloat);
								cube.addQuad(Enums::Direction::NORTH, colorFloat);
								cube.add(vertext, UVt, colort, AOt, indiciest, countVertexTransperent);
							}
						}
					}
				}
			}
		}
	}
	buffer = !vertex.empty();
	buffert = !vertext.empty();
}

void Section::loadBuffer()
{
	if (buffer)
	{
		buffer = false;
		vao.loadData(0, vertex);
		vao.loadData(1, color);
		vao.loadData(2, UV);
		vao.loadData(3, AO, GL_UNSIGNED_BYTE);
		vao.loadIndices(indicies);
		vertex.clear();
		color.clear();
		UV.clear();
		AO.clear();
		indicies.clear();
		visible = true;
	}
	if (buffert)
	{
		buffert = false;
		vaoTransperent.loadData(0, vertext);
		vaoTransperent.loadData(1, colort);
		vaoTransperent.loadData(2, UVt);
		vaoTransperent.loadData(3, AOt, GL_UNSIGNED_BYTE);
		vaoTransperent.loadIndices(indiciest);
		vertext.clear();
		colort.clear();
		UVt.clear();
		AOt.clear();
		indiciest.clear();
		visiblet = true;
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
	if (buffert)
		loadBuffer();
	if (visiblet)
		vaoTransperent.draw();
}