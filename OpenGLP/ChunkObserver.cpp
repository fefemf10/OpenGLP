#include "ChunkObserver.hpp"

#include "DiskSystem.hpp"
#include "World.hpp"
#include "State.hpp"

ChunkObserver::ChunkObserver(World& world) : world(world)
{
}

void ChunkObserver::handle(const Event* event)
{
	if (event->type() == Events::ChunkNeedLoadEvent)
	{
		const ChunkNeedLoadEvent* chunkNeedLoadEvent = static_cast<const ChunkNeedLoadEvent*>(event);
		const glm::ivec2 position = chunkNeedLoadEvent->position;
		std::async(std::launch::async, [&]()
			{
				const glm::ivec2& local = world.getLocalPositionChunk(position);
				if (!world.validateLocalPosChunk(local))
					return;
				DiskSystem::loadChunk(position);
				Chunk& chunk = *(world.chunks[local.y][local.x]);
				chunk.work.wait(true);
				chunk.setWork(true);
				nbt::NBT nbtChunk = DiskSystem::getChunk(position);
				nbt::TagCompound& heightMaps = nbtChunk.at<nbt::TagCompound>("Heightmaps");
				chunk.motion_blocking = heightMaps.at<nbt::TagLongArray>("MOTION_BLOCKING");
				chunk.motion_blocking_no_leaves = heightMaps.at<nbt::TagLongArray>("MOTION_BLOCKING_NO_LEAVES");
				chunk.ocean_floor = heightMaps.at<nbt::TagLongArray>("OCEAN_FLOOR");
				chunk.world_surface = heightMaps.at<nbt::TagLongArray>("WORLD_SURFACE");
				chunk.position.x = nbtChunk.at<nbt::TagInt>("xPos");
				chunk.position.y = nbtChunk.at<nbt::TagInt>("yPos");
				chunk.position.z = nbtChunk.at<nbt::TagInt>("zPos");
				chunk.localPosition = local;
				std::vector<nbt::TagCompound>& sections = nbt::get_list<nbt::TagCompound>(nbtChunk.at<nbt::TagList>("sections"));
				chunk.sectionCount = glm::min((int)sections.size(), 25);
				for (size_t i = 0; i < glm::min((int)sections.size(), 25); ++i)
				{
					if (sections[i].base.contains("BlockLight"))
						chunk.sections[i].blockLight = sections[i].at<nbt::TagByteArray>("BlockLight");
					else
						chunk.sections[i].blockLight = std::vector<int8_t>(2048, 0);
					if (sections[i].base.contains("SkyLight"))
						chunk.sections[i].skyLight = sections[i].at<nbt::TagByteArray>("SkyLight");
					else
						chunk.sections[i].skyLight = std::vector<int8_t>(2048, 0);
					{
						int h = sections[i].at<nbt::TagByte>("Y");
						chunk.sections[i].position = { position.x, h, position.y };
					}
					if (sections[i].base.contains("block_states"))
					{
						nbt::TagCompound& blockStates = sections[i].at<nbt::TagCompound>("block_states");
						nbt::TagCompound& biomes = sections[i].at<nbt::TagCompound>("biomes");
						if (biomes.base.contains("data"))
						{
							chunk.sections[i].dataBiome = biomes.at<nbt::TagLongArray>("data");
						}
						else
						{
							chunk.sections[i].dataBiome = std::vector<int64_t>(1, 0);
						}
						if (blockStates.base.contains("data"))
						{
							chunk.sections[i].dataBlock = blockStates.at<nbt::TagLongArray>("data");
						}
						else
						{
							chunk.sections[i].dataBlock = std::vector<int64_t>(256, 0);
						}
						std::vector<nbt::TagCompound>& blockPalette = nbt::get_list<nbt::TagCompound>(blockStates.at<nbt::TagList>("palette"));
						std::vector<nbt::TagString>& biomePalette = nbt::get_list<nbt::TagString>(biomes.at<nbt::TagList>("palette"));
						chunk.sections[i].blockPalette.resize(blockPalette.size());
						chunk.sections[i].biomePalette.resize(biomePalette.size());
						const int8_t bitsPerBlock = std::max(4, static_cast<int>(std::ceilf(std::log2f(static_cast<float>(blockPalette.size())))));
						const int8_t bitsPerBiome = std::max(1, static_cast<int>(std::ceilf(std::log2f(static_cast<float>(biomePalette.size())))));
						chunk.sections[i].bitsPerBlock = bitsPerBlock;
						chunk.sections[i].bitsPerBiome = bitsPerBiome;
						chunk.sections[i].countIndexPerLongBlock = 64u / bitsPerBlock;
						chunk.sections[i].countIndexPerLongBiome = 64u / bitsPerBiome;
						chunk.sections[i].maskBlock = (1ull << bitsPerBlock) - 1ull;
						chunk.sections[i].maskBiome = (1ull << bitsPerBiome) - 1ull;

						for (size_t j = 0; j < biomePalette.size(); ++j)
						{
							chunk.sections[i].biomePalette[j] = Enums::iBiome(biomePalette[j].substr(10));
						}
						for (size_t j = 0; j < blockPalette.size(); ++j)
						{
							std::string pkey = blockPalette[j].at<nbt::TagString>("Name").substr(10);
							chunk.sections[i].blockPalette[j].id = Enums::iBlock(pkey);
							if (blockPalette[j].base.contains("Properties"))
							{
								for (auto& [key, value] : blockPalette[j].at<nbt::TagCompound>("Properties").base)
								{
									std::string nameProp = key;
									Enums::PropertiesBlock propBlock = Enums::iPropertiesBlock(nameProp);
									nameProp = std::get<nbt::TagString>(value);
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
									chunk.sections[i].blockPalette[j].properties.insert({ propBlock, value });
								}
							}
						}
					}
				}
				//if (!chunk.loadChunk(pos, worldDir))
				//{
				//	//poolChunks.enqueue(std::bind(&Chunk::generateChunk, chunks[local.y][local.x], pos, seed));
				//	
				//}
				//chunk.findNeighbors();
				chunk.setModified(true);
				chunk.state.store(State::Modified);
				chunk.setWork(false);
				/*if ((local.x >= 0 && local.x < a) && (local.y - 1 >= 0 && local.y - 1 < a))
				{
					chunks[local.y - 1][local.x]->setModified(true);
					if (chunks[local.y - 1][local.x]->isModified() && !chunks[local.y - 1][local.x]->isWork())
					{
						poolChunks.enqueue(std::bind(&Chunk::genMesh, chunks[local.y - 1][local.x]));
					}
				}
				if ((local.x >= 0 && local.x < a) && (local.y + 1 >= 0 && local.y + 1 < a))
				{
					chunks[local.y + 1][local.x]->setModified(true);
					if (chunks[local.y + 1][local.x]->isModified() && !chunks[local.y + 1][local.x]->isWork())
					{
						poolChunks.enqueue(std::bind(&Chunk::genMesh, chunks[local.y + 1][local.x]));
					}
				}
				if ((local.x - 1 >= 0 && local.x - 1 < a) && (local.y >= 0 && local.y < a))
				{
					chunks[local.y][local.x - 1]->setModified(true);
					if (chunks[local.y][local.x - 1]->isModified() && !chunks[local.y][local.x - 1]->isWork())
					{
						poolChunks.enqueue(std::bind(&Chunk::genMesh, chunks[local.y][local.x - 1]));
					}
				}
				if ((local.x + 1 >= 0 && local.x + 1 < a) && (local.y >= 0 && local.y < a))
				{
					chunks[local.y][local.x + 1]->setModified(true);
					if (chunks[local.y][local.x + 1]->isModified() && !chunks[local.y][local.x + 1]->isWork())
					{
						poolChunks.enqueue(std::bind(&Chunk::genMesh, chunks[local.y][local.x + 1]));
					}
				}*/
			});
	}
	else if (event->type() == Events::ChunkNeedMeshEvent)
	{
		const ChunkNeedMeshEvent* chunkNeedMeshEvent = static_cast<const ChunkNeedMeshEvent*>(event);
		glm::ivec2 position = chunkNeedMeshEvent->position;
		std::async(std::launch::async, [&]()
			{
				const glm::ivec2& local = world.getLocalPositionChunk(position);
				if (!world.validateLocalPosChunk(local))
					return;
				Chunk& chunk = *(world.chunks[local.y][local.x]);
				chunk.work.wait(true);
				chunk.setWork(true);
				if (chunk.state.load() == State::Modified)
				{
					chunk.genMesh();
					chunk.state.store(State::Buffered);
					world.genMeshq.enqueue(local);
				}
				chunk.setWork(false);
			});
	}
	else if (event->type() == Events::ChunkNeedBufferEvent)
	{
		const ChunkNeedBufferEvent* chunkNeedBufferEvent = static_cast<const ChunkNeedBufferEvent*>(event);
		const glm::ivec2 position = chunkNeedBufferEvent->position;

		const glm::ivec2& local = world.getLocalPositionChunk(position);
		if (!world.validateLocalPosChunk(local))
			return;
		Chunk& chunk = *(world.chunks[local.y][local.x]);
		chunk.work.wait(true);
		chunk.setWork(true);
		for (size_t i = 1; i < chunk.sectionCount; i++)
		{
			SectionMesh& sectionMesh = *(world.sectionMesh[local.y][local.x][i]);
			Section& section = chunk.sections[i];
			sectionMesh.load(section.vertex, section.color, section.UV, section.AO, section.indicies);
			sectionMesh.position = section.position;
			section.loadBuffer();
		}
		chunk.state.store(State::Full);
		chunk.setWork(false);
	}
}