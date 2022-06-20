#include "World.hpp"
#include "DiskSystem.hpp"
#include "Logger.hpp"
#include "State.hpp"

World::World(Player& player, Camera& camera) : player(player), camera(camera), poolChunks(std::thread::hardware_concurrency()), chunkObserver(*this), worldInfo(DiskSystem::worldInfo)
{
	player.setPosition({ worldInfo.spawnX, worldInfo.spawnY, worldInfo.spawnZ, });
	int a = player.getRenderDistance() * 2 + 1;
	chunks.resize(a, std::vector<Chunk*>(a, nullptr));
	sectionMesh.resize(a, std::vector<std::vector<SectionMesh*>>(a, std::vector<SectionMesh*>(25, nullptr)));
	genMeshq.update(glm::ivec2(a) / 2);
	for (int z = 0; z < a; z++)
	{
		for (int x = 0; x < a; x++)
		{
			chunks[z][x] = new Chunk();
			chunks[z][x]->sections.resize(25, Section(*this));
			for (size_t y = 0; y < 25; y++)
			{
				sectionMesh[z][x][y] = new SectionMesh();
				sectionMesh[z][x][y]->init();
			}
		}
	}
	endLocalPosChunk = { a - 1, a - 1 };
	loadChunks();
	firstLoad = false;
	dispatcher.subscribe(Events::ChunkLoadedEvent, std::bind(&ChunkObserver::handle, chunkObserver, std::placeholders::_1));
	dispatcher.subscribe(Events::ChunkMeshedEvent, std::bind(&ChunkObserver::handle, chunkObserver, std::placeholders::_1));
	dispatcher.subscribe(Events::ChunkNeedLoadEvent, std::bind(&ChunkObserver::handle, chunkObserver, std::placeholders::_1));
	dispatcher.subscribe(Events::ChunkNeedMeshEvent, std::bind(&ChunkObserver::handle, chunkObserver, std::placeholders::_1));
	dispatcher.subscribe(Events::ChunkNeedBufferEvent, std::bind(&ChunkObserver::handle, chunkObserver, std::placeholders::_1));
}

void World::saveWorld()
{
	poolChunks.stop();
	const int a = player.getRenderDistance() * 2 + 1;
	for (int z = 0; z < a; z++)
	{
		for (int x = 0; x < a; x++)
		{
			delete chunks[z][x];
			for (size_t y = 0; y < 25; y++)
			{
				delete sectionMesh[z][x][y];
			}
		}
	}
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
	const int xp = static_cast<int32_t>(player.getPosition().x);
	const int zp = static_cast<int32_t>(player.getPosition().z);
	const glm::ivec2 playerPos(xp >> 4, zp >> 4);
	const unsigned char renderDistance = player.getRenderDistance();
	const int a = renderDistance * 2 + 1;
	countChunks = a * a;
	genMeshq.update(playerPos);
	if (firstLoad)
	{
		for (int z = 0; z < a; ++z)
		{
			for (int x = 0; x < a; ++x)
			{
				const glm::ivec2& position{ playerPos.x - renderDistance + x, playerPos.y - renderDistance + z };
				dispatcher.post(new ChunkNeedLoadEvent(position));
				dispatcher.post(new ChunkNeedMeshEvent(position));
				dispatcher.post(new ChunkNeedBufferEvent(position));
				//const glm::ivec2 position{ playerPos.x + x, playerPos.y + z };
				//loadq.enqueue(position);
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
		std::vector<std::vector<std::vector<SectionMesh*>>> tmpm(a, std::vector<std::vector<SectionMesh*>>(a, std::vector<SectionMesh*>(25, nullptr)));
		std::vector<std::vector<bool>> tmpCheck(a, std::vector<bool>(a));
		std::vector<std::vector<bool>> tmpCheck2(a, std::vector<bool>(a));
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
				tmpm[z - deltaPos.y][x - deltaPos.x] = sectionMesh[z][x];
				tmpCheck[z][x] = true;
			}
		}
		if (deltaPos.x >= 1)
		{
			minX = deltaPos.x + 1;
			maxX = a;
		}
		else if (deltaPos.x <= -1)
		{
			minX = 0;
			maxX = a + deltaPos.x - 1;
		}
		else
		{
			minX = 0;
			maxX = a;
		}
		if (deltaPos.y >= 1)
		{
			minZ = deltaPos.y + 1;
			maxZ = a;
		}
		else if (deltaPos.y <= -1)
		{
			minZ = 0;
			maxZ = a + deltaPos.y - 1;
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
				tmpCheck2[z][x] = true;
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
					tmpm[a - z - 1][a - x - 1] = sectionMesh[z][x];
					for (size_t y = 0; y < 25; y++)
					{
						tmp[a - z - 1][a - x - 1]->sections[y].loadBuffer();
						tmpm[a - z - 1][a - x - 1][y]->clear();
					}
					loadq.enqueue(chunkPos);
				}
				if (!tmpCheck2[z][x] && tmpCheck[z][x])
				{
					chunks[a - z - 1][a - x - 1]->setModified(true);
					genMeshq.enqueue({ a - x - 1, a - z - 1 });
				}
			}
		}
		chunks = tmp;
		sectionMesh = tmpm;
	}*/
	playerLastPosition = playerPos;
}

uint64_t World::getSeed() const
{
	return seed;
}

glm::ivec2 World::getLocalPositionChunk(const glm::ivec2& position) const
{
	return position - glm::ivec2(player.getPosition().x - player.getRenderDistance(), player.getPosition().z - player.getRenderDistance());
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
	const int x = static_cast<int32_t>(player.getPosition().x);
	const int z = static_cast<int32_t>(player.getPosition().z);
	const glm::ivec2 playerPos(x >> 4, z >> 4);
	const unsigned char renderDistance = player.getRenderDistance();
	//drawq.update(playerPos, false);
	const int a = renderDistance * 2 + 1;
	if (playerLastPosition != playerPos)
		loadChunks();

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
	dispatcher.process();
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
	//for (size_t z = 0; z < chunks.size(); ++z)
	//{
	//	for (size_t x = 0; x < chunks.size(); ++x)
	//	{
	//		for (size_t y = 1; y < chunks[z][x]->sectionCount; y++)
	//		{
	//			Section& section = chunks[z][x]->sections[y];
	//			if (section.buffer.load() && !section.work.load())
	//			{
	//				sectionMesh[z][x][y]->load(section.vertex, section.color, section.UV, section.AO, section.indicies);
	//				sectionMesh[z][x][y]->position = section.position;
	//				section.loadBuffer();
	//			}
	//			/*const int f = camera.getFrustum().CubeInFrustum(glm::vec3(sectionMesh[z][x][y]->position) * 16.f + 8.f, glm::vec3(8));
	//			if (f == Frustum::FRUSTUM_INTERSECT || f == Frustum::FRUSTUM_INSIDE)
	//			{
	//				sectionMesh[z][x][y]->draw();
	//				if (sectionMesh[z][x][y]->getCountVertex())
	//				{
	//					countVertex += sectionMesh[z][x][y]->getCountVertex();
	//					countVisibleSection++;
	//				}
	//			}*/
	//		}
	//	}
	//}
	const int xp = static_cast<int32_t>(player.getPosition().x);
	const int yp = static_cast<int32_t>(player.getPosition().y);
	const int zp = static_cast<int32_t>(player.getPosition().z);
	const glm::ivec3 playerPos(xp >> 4, yp >> 4, zp >> 4);
	const int a = player.getRenderDistance() * 2 + 1;
	glm::ivec3 startSection(playerPos.x - player.getRenderDistance(), 0, playerPos.z - player.getRenderDistance());
	glm::ivec3 local = playerPos - glm::ivec3(startSection.x, -5, startSection.z);
	
	for (size_t z = 0; z < chunks.size(); ++z)
	{
		for (size_t x = 0; x < chunks.size(); ++x)
		{
			for (size_t y = 1; y < chunks[z][x]->sectionCount; y++)
			{
				const int f = camera.getFrustum().CubeInFrustum(glm::vec3(sectionMesh[z][x][y]->position) * 16.f + 8.f, glm::vec3(8));
				if (f == Frustum::FRUSTUM_INSIDE || f == Frustum::FRUSTUM_INTERSECT)
				{
					sectionMesh[z][x][y]->draw();
					if (sectionMesh[z][x][y]->getCountVertex())
					{
						countVertex += sectionMesh[z][x][y]->getCountVertex();
						countVisibleSection++;
					}
				}
			}
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
	///*glEnable(GL_BLEND);
	//while (ddrawq.count() > 0)
	//{
	//	const glm::ivec2 pos = ddrawq.dequeue();
	//	chunks[pos.y][pos.x]->drawTransperent(camera.getFrustum());
	//	countVertexTransperent += chunks[pos.y][pos.x]->countVertexTransperent;
	//}*/
	allVertex = countVertex + countVertexTransperent;
}

bool World::validateLocalPosChunk(const glm::ivec2& position) const
{
	return !(position.x < 0 || position.y < 0 || position.x > endLocalPosChunk.x || position.y > endLocalPosChunk.y);
}

bool World::loadedNeighbours(const glm::ivec2& position)
{
	bool f{ true };
	if (validateLocalPosChunk(glm::ivec2(position.x + 1, position.y)))
		f &= chunks[position.y][position.x + 1]->state.load() != State::Empty;
	if (validateLocalPosChunk(glm::ivec2(position.x - 1, position.y)))
		f &= chunks[position.y][position.x - 1]->state.load() != State::Empty;
	if (validateLocalPosChunk(glm::ivec2(position.x, position.y + 1)))
		f &= chunks[position.y + 1][position.x]->state.load() != State::Empty;
	if (validateLocalPosChunk(glm::ivec2(position.x, position.y - 1)))
		f &= chunks[position.y - 1][position.x]->state.load() != State::Empty;
	return f;
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