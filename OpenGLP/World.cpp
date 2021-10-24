#include "World.hpp"

World::World(const std::string& name, Player& player, Camera& camera) : player(player), camera(camera), poolChunks(std::thread::hardware_concurrency())
{
	if (!std::filesystem::exists(std::filesystem::current_path() / "saves"))
		std::filesystem::create_directory(std::filesystem::current_path() / "saves");
	std::filesystem::path worldDir = std::filesystem::current_path() / "saves" / name;
	if (!std::filesystem::exists(worldDir))
		std::filesystem::create_directory(worldDir);
	if (std::filesystem::exists(worldDir / "level.dat"))
	{
		level.deserialize((worldDir / "level.dat").string(), true);
	}
	else
	{

	}
	nbt::TagCompound data = level.root.at<nbt::TagCompound>("Data");
	player.setPosition({ data.at<nbt::TagInt>("SpawnX"), data.at<nbt::TagInt>("SpawnY"), data.at<nbt::TagInt>("SpawnZ") });
	this->worldDir = worldDir;
	int a = player.getRenderDistance() * 2 + 1;
	chunks.resize(a, std::vector<Chunk*>(a, nullptr));
	for (int z = 0; z < a; z++)
	{
		for (int x = 0; x < a; x++)
		{
			chunks[z][x] = new Chunk(chunks);
		}
	}
	endLocalPosChunk = {a-1, a-1};
	loadChunks();
	firstLoad = false;
}

void World::saveWorld()
{
	poolChunks.stop();
}

std::string World::getBlock(const glm::ivec3& position)
{
	return "minecraft:air";
}

Chunk* World::getChunk(const glm::ivec2& position)
{
	//glm::ivec2 localPos = position - chunks[0][0]->position;
	int x = static_cast<int32_t>(player.getPosition().x);
	int z = static_cast<int32_t>(player.getPosition().z);
	glm::ivec2 playerPos = glm::ivec2(x >> 4, z >> 4);
	glm::ivec2 startChunk = playerPos - glm::ivec2(player.getRenderDistance());
	glm::ivec2 localPos = position - startChunk;
	if (validateLocalPosChunk(localPos))
		return chunks[localPos.y][localPos.x];
	else
		return nullptr;
}

Section* World::getSection(const glm::ivec3& position)
{
	Chunk* chunk = getChunk({ position.x, position.z });
	if (chunk)
		return chunk->getSection(position.y);
	else
		return nullptr;
}

void World::setBlock(const glm::ivec3& position, const std::string& id)
{

}

void World::loadChunks()
{
	int xp = static_cast<int32_t>(player.getPosition().x);
	int zp = static_cast<int32_t>(player.getPosition().z);
	glm::ivec2 playerPos = glm::ivec2(xp >> 4, zp >> 4);
	unsigned char renderDistance = player.getRenderDistance();
	int a = renderDistance * 2 + 1;
	countChunks = a * a;
	loadq.update(playerPos);
	if (firstLoad)
	{
		for (int z = 0; z < a; ++z)
		{
			for (int x = 0; x < a; ++x)
			{
				const glm::ivec2 position{ playerPos.x - renderDistance + x, playerPos.y - renderDistance + z };
				//const glm::ivec2 position{ playerPos.x + x, playerPos.y + z };
				loadq.enqueue(position);
				drawq.enqueue({ x, z }, false);
				/*chunks[z][x]->seed = seed;
				if (!chunks[z][x]->loadChunk(position, worldDir))
				{
					poolChunks.enqueue(std::bind(&Chunk::generateChunk, chunks[z][x], position, seed));
				}*/
			}
		}
	}
	/*else
	{
		glm::ivec2 deltaPos = playerPos - playerLastPosition;
		std::vector<std::vector<Chunk*>> tmp(a, std::vector<Chunk*>(a, nullptr));
		std::vector<std::vector<bool>> tmpCheck(a, std::vector<bool>(a));
		size_t minX{};
		size_t maxX{};
		size_t minZ{};
		size_t maxZ{};
		if (deltaPos.x >= 1)
		{
			minX = deltaPos.x;
			maxX = a;
		}
		else if (deltaPos.x <= -1)
		{
			minX = 0;
			maxX = a + deltaPos.x;
		}
		else
		{
			minX = 0;
			maxX = a;
		}
		if (deltaPos.y >= 1)
		{
			minZ = deltaPos.y;
			maxZ = a;
		}
		else if (deltaPos.y <= -1)
		{
			minZ = 0;
			maxZ = a + deltaPos.y;
		}
		else
		{
			minZ = 0;
			maxZ = a;
		}
		for (size_t z = minZ; z < maxZ; ++z)
		{
			for (size_t x = minX; x < maxX; ++x)
			{
				tmp[z - deltaPos.y][x - deltaPos.x] = chunks[z][x];
				tmpCheck[z][x] = true;
			}
		}
		for (size_t z = 0; z < a; ++z)
		{
			for (size_t x = 0; x < a; ++x)
			{
				if (!tmpCheck[z][x])
				{
					const glm::ivec2 chunkPos(playerPos + glm::ivec2(renderDistance) - glm::ivec2(x, z));
					tmp[a - z - 1][a - x - 1] = chunks[z][x];
					loadq.enqueue(chunkPos);
				}
			}
		}
		chunks = tmp;
	}*/
	playerLastPosition = playerPos;
}

uint64_t World::getSeed() const
{
	return seed;
}

unsigned short World::getCountChunks()
{
	return countChunks;
}

void World::update(const float& dt)
{
	tick += dt;
	if (tick >= (1.f / 20.f))
	{
		updateTick();
		tick = 0.f;
	}
	int x = static_cast<int32_t>(player.getPosition().x);
	int z = static_cast<int32_t>(player.getPosition().z);
	glm::ivec2 playerPos = glm::ivec2(x >> 4, z >> 4);
	unsigned char renderDistance = player.getRenderDistance();
	//drawq.update(playerPos, false);
	int a = renderDistance * 2 + 1;
	if (playerLastPosition != playerPos)
		loadChunks();
	//if (loadq.count() > 5)
	//{
	//	for (size_t i = 0; i < 6; i++)
	//	{
	//		glm::ivec2 pos = loadq.dequeue();
	//		glm::ivec2 startChunk = playerPos - glm::ivec2(player.getRenderDistance());
	//		glm::uvec2 local = pos - startChunk;
	//		if ((local.x >= 0 && local.x < a) && (local.y >= 0 && local.y < a))
	//		{
	//			chunks[local.y][local.x]->seed = seed;
	//			//if (!chunks[local.y][local.x]->loadChunk(pos, worldDir))
	//			//{
	//			//	//poolChunks.enqueue(std::bind(&Chunk::generateChunk, chunks[local.y][local.x], pos, seed));
	//			//	
	//			//}
	//			chunks[local.y][local.x]->generateChunk(pos, seed);
	//			chunks[local.y][local.x]->genMesh();
	//		}
	//	}
	//}
	while (loadq.count() > 0)
	{
		glm::ivec2 pos = loadq.dequeue();
		glm::ivec2 startChunk = playerPos - glm::ivec2(player.getRenderDistance());
		glm::uvec2 local = pos - startChunk;
		glm::ivec2 regionPos = pos >> 5;
		glm::ivec2 chunkPos = (glm::ivec2(32) + pos) % 32;

		std::fstream region(worldDir / "region" / std::string("r." + std::to_string(regionPos.x) + "." + std::to_string(regionPos.y) + ".mca"), std::ios::in | std::ios::binary);
		if (region.is_open())
		{
			regions[regionPos].deserialize(region, chunkPos.x, chunkPos.y);
		}
		region.close();
		if ((local.x >= 0 && local.x < a) && (local.y >= 0 && local.y < a))
		{
			nbt::NBT chunk = regions[regionPos].chunks[chunkPos.y * 32 + chunkPos.x];
			nbt::TagCompound& level = chunk.at<nbt::TagCompound>("Level");
			nbt::TagCompound& heightMaps = level.at<nbt::TagCompound>("Heightmaps");
			chunks[local.y][local.x]->motion_blocking = heightMaps.at<nbt::TagLongArray>("MOTION_BLOCKING");
			chunks[local.y][local.x]->motion_blocking_no_leaves = heightMaps.at<nbt::TagLongArray>("MOTION_BLOCKING_NO_LEAVES");
			chunks[local.y][local.x]->ocean_floor = heightMaps.at<nbt::TagLongArray>("OCEAN_FLOOR");
			chunks[local.y][local.x]->world_surface = heightMaps.at<nbt::TagLongArray>("WORLD_SURFACE");
			chunks[local.y][local.x]->position.x = level.at<nbt::TagInt>("xPos");
			chunks[local.y][local.x]->position.y = level.at<nbt::TagInt>("zPos");
			chunks[local.y][local.x]->localPosition = local;
			std::vector<nbt::TagCompound>& sections = nbt::get_list<nbt::TagCompound>(level.at<nbt::TagList>("Sections"));
			chunks[local.y][local.x]->sections.resize(sections.size(), Section(*this));
			for (size_t i = 0; i < sections.size(); ++i)
			{
				if (sections[i].base.contains("BlockLight"))
					chunks[local.y][local.x]->sections[i].blockLight = sections[i].at<nbt::TagByteArray>("BlockLight");
				else
					chunks[local.y][local.x]->sections[i].blockLight = std::vector<int8_t>(2048, 0);
				if (sections[i].base.contains("SkyLight"))
					chunks[local.y][local.x]->sections[i].skyLight = sections[i].at<nbt::TagByteArray>("SkyLight");
				else
					chunks[local.y][local.x]->sections[i].skyLight = std::vector<int8_t>(2048, 0);
				{
					int h = sections[i].at<nbt::TagByte>("Y");
					chunks[local.y][local.x]->sections[i].position = { pos.x, h, pos.y };
				}
				nbt::TagCompound& blockStates = sections[i].at<nbt::TagCompound>("block_states");
				nbt::TagCompound& biomes = sections[i].at<nbt::TagCompound>("biomes");
				if (blockStates.base.contains("data"))
				{
					chunks[local.y][local.x]->sections[i].dataBlock = blockStates.at<nbt::TagLongArray>("data");
				}
				if (biomes.base.contains("data"))
				{
					chunks[local.y][local.x]->sections[i].dataBiome = biomes.at<nbt::TagLongArray>("data");
				}
				else
				{
					chunks[local.y][local.x]->sections[i].dataBiome = std::vector<int64_t>(1, 0);
				}
				std::vector<nbt::TagCompound>& blockPalette = nbt::get_list<nbt::TagCompound>(blockStates.at<nbt::TagList>("palette"));
				std::vector<nbt::TagString>& biomePalette = nbt::get_list<nbt::TagString>(biomes.at<nbt::TagList>("palette"));
				chunks[local.y][local.x]->sections[i].blockPalette.resize(blockPalette.size());
				chunks[local.y][local.x]->sections[i].biomePalette.resize(biomePalette.size());
				const int8_t bitsPerBlock = std::max(4, static_cast<int>(std::ceilf(std::log2f(static_cast<float>(blockPalette.size())))));
				const int8_t bitsPerBiome = std::max(1, static_cast<int>(std::ceilf(std::log2f(static_cast<float>(biomePalette.size())))));
				chunks[local.y][local.x]->sections[i].bitsPerBlock = bitsPerBlock;
				chunks[local.y][local.x]->sections[i].bitsPerBiome = bitsPerBiome;
				chunks[local.y][local.x]->sections[i].countIndexPerLongBlock = 64u / bitsPerBlock;
				chunks[local.y][local.x]->sections[i].countIndexPerLongBiome = 64u / bitsPerBiome;
				chunks[local.y][local.x]->sections[i].maskBlock = (1ull << bitsPerBlock) - 1ull;
				chunks[local.y][local.x]->sections[i].maskBiome = (1ull << bitsPerBiome) - 1ull;

				for (size_t j = 0; j < biomePalette.size(); ++j)
				{
					chunks[local.y][local.x]->sections[i].biomePalette[j] = Enums::iBiome(biomePalette[j].substr(10));
				}
				for (size_t j = 0; j < blockPalette.size(); ++j)
				{
					std::string pkey = blockPalette[j].at<nbt::TagString>("Name").substr(10);
					chunks[local.y][local.x]->sections[i].blockPalette[j].id = Enums::iBlock(pkey);
					if (blockPalette[j].base.contains("Properties"))
					{
						std::map<Enums::PropertiesBlock, uint8_t> p;
						for (auto& [key, value] : blockPalette[j].at<nbt::TagCompound>("Properties").base)
						{
							std::string nameProp = key;
							Enums::PropertiesBlock propBlock = Enums::iPropertiesBlock(nameProp);
							nameProp = std::get<nbt::TagString>(value);
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
						chunks[local.y][local.x]->sections[i].blockPalette[j].properties = p;
					}
				}
			}
			//if (!chunks[local.y][local.x]->loadChunk(pos, worldDir))
			//{
			//	//poolChunks.enqueue(std::bind(&Chunk::generateChunk, chunks[local.y][local.x], pos, seed));
			//	
			//}
			//chunks[local.y][local.x]->findNeighbors();
			chunks[local.y][local.x]->modified = true;
			if ((local.x >= 0 && local.x < a) && (local.y - 1 >= 0 && local.y - 1 < a))
			{
				//chunks[local.y - 1][local.x]->findNeighbors();
				chunks[local.y - 1][local.x]->modified = true;
				//chunks[local.y - 1][local.x]->genMesh();
			}
			if ((local.x >= 0 && local.x < a) && (local.y + 1 >= 0 && local.y + 1 < a))
			{
				//chunks[local.y + 1][local.x]->findNeighbors();
				chunks[local.y + 1][local.x]->modified = true;
				//chunks[local.y + 1][local.x]->genMesh();
			}
			if ((local.x - 1 >= 0 && local.x - 1 < a) && (local.y >= 0 && local.y < a))
			{
				//chunks[local.y][local.x - 1]->findNeighbors();
				chunks[local.y][local.x - 1]->modified = true;
				//chunks[local.y][local.x - 1]->genMesh();
			}
			if ((local.x + 1 >= 0 && local.x + 1 < a) && (local.y >= 0 && local.y < a))
			{
				//chunks[local.y][local.x + 1]->findNeighbors();
				chunks[local.y][local.x + 1]->modified = true;
				//chunks[local.y][local.x + 1]->genMesh();
			}
			//chunks[local.y][local.x]->genMesh();
		}
	}
	if (player.fly)
	{
		if (ImGui::IsKeyDown(GLFW_KEY_W))
		{
			player.move(player.getLook() * player.speed * 4.f * dt);
		}
		if (ImGui::IsKeyDown(GLFW_KEY_S))
		{
			player.move(player.getLook() * -player.speed * 4.f * dt);
		}
		if (ImGui::IsKeyDown(GLFW_KEY_A))
		{
			player.move(player.getRight() * player.speed * 4.f * dt);
		}
		if (ImGui::IsKeyDown(GLFW_KEY_D))
		{
			player.move(player.getRight() * -player.speed * 4.f * dt);
		}
	}
}

void World::updateTick()
{
	TextureAtlas& atlas = RM::getAtlas("blocks");
	atlas.updateTick();
}

void World::draw()
{
	allVertex = 0;
	countVertex = 0;
	countVertexTransperent = 0;
	countVisibleSection = 0;
	queuep ddrawq = drawq;
	//glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	for (size_t z = 0; z < chunks.size(); ++z)
	{
		for (size_t x = 0; x < chunks.size(); ++x)
		{
			if (chunks[z][x]->modified)
				chunks[z][x]->genMesh();
			chunks[z][x]->draw(camera.getFrustum());
			countVertex += chunks[z][x]->countVertex;
			countVisibleSection += chunks[z][x]->countVisibleSection;
		}
	}
	/*for (size_t z = 0; z < chunks.size(); ++z)
	{
		for (size_t x = 0; x < chunks.size(); ++x)
		{
			chunks[z][x]->drawTransperent(camera.getFrustum());
			countVertexTransperent += chunks[z][x]->countVertexTransperent;
		}
	}*/
	glEnable(GL_BLEND);
	while (ddrawq.count() > 0)
	{
		const glm::ivec2 pos = ddrawq.dequeue();
		chunks[pos.y][pos.x]->drawTransperent(camera.getFrustum());
		countVertexTransperent += chunks[pos.y][pos.x]->countVertexTransperent;
	}
	allVertex = countVertex + countVertexTransperent;
}

bool World::validateLocalPosChunk(const glm::ivec2& position)
{
	return !(position.x < 0 || position.y < 0 || position.x > endLocalPosChunk.x || position.y > endLocalPosChunk.y);
}

std::string World::rayCast(const glm::vec3& pos, const glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
{
	float px = pos.x;
	float py = pos.y;
	float pz = pos.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

	float stepx = (dx > 0.0f) ? 1.0f : (dx < 0.0f) ? -1.0f : 0.f;
	float stepy = (dy > 0.0f) ? 1.0f : (dy < 0.0f) ? -1.0f : 0.f;
	float stepz = (dz > 0.0f) ? 1.0f : (dz < 0.0f) ? -1.0f : 0.f;

	constexpr float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;

	while (t <= maxDist) {
		const std::string voxel = getBlock(glm::vec3(ix, iy, iz));
		if (voxel == "minecraft:air") {
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;
			return voxel;
		}
		if (txMax < tyMax) {
			if (txMax < tzMax) {
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			}
			else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
		else {
			if (tyMax < tzMax) {
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			}
			else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return "minecraft:air";
}