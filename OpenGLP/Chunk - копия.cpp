#include "Chunk.hpp"

Chunk::Chunk(std::vector<std::vector<Chunk*>>& chunks) : blocks{ Block::AIR }, chunks(chunks), maxHeightBlock{}, countVertex(), modified()
{

}

Chunk::~Chunk()
{
}

void Chunk::generateChunk(const glm::ivec2& position, const double& seed)
{
	this->position = position;
	modified = true;
	saving = true;
#pragma omp parallel for
	for (int z = 0; z < 16; ++z)
		for (int x = 0; x < 16; ++x)
		{
			blocks[0][z][x] = Block(Block::BEDROCK);
			unsigned char y = static_cast<unsigned char>((glm::simplex(glm::vec3((x + position.x * 16) * 0.025f, (float)seed, (z + position.y * 16) * 0.025f)) + 1.f) * 24.f + 48);
			for (int i = 1; i < y; ++i)
			{
				blocks[i][z][x] = Block(Block::STONE);
			}
			if (y > maxHeightBlock)
				maxHeightBlock = y;
		}

#pragma omp parallel for
	for (int y = 1; y < 64; ++y)
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int x = 0; x < 16; ++x)
			{
				float p = glm::simplex(glm::vec4((x + position.x * 16) * 0.025f, y * 0.025f, (z + position.y * 16) * 0.025f, (float)seed));
				/*if (p > 0.75f)
					blocks[y][z][x] = Block(Block::DIRT);
				else if (p > 0.f)
					blocks[y][z][x] = Block(Block::STONE);*/
				if (p < 0.f)
					blocks[y][z][x] = Block(Block::Type::AIR);
				//i = glm::perlin(glm::vec3((x + position.x * 16) / 64.f, (z + position.y * 16) / 64.f, y / 127.f), glm::vec3(seed));// + 1.f) * 16.f + 32;
				//if (i > 0.0f)
				//	blocks[y][z][x] = Block(Block::AIR);

				/*if (y > maxHeightBlock)
					maxHeightBlock = y;*/

					/*for (unsigned char i = 0; i < y; ++i)
					{
						if (i > 60)
							blocks[i][z][x] = Block(Block::GRASS);
						else
							blocks[i][z][x] = Block(Block::STONE);
					}*/
			}
		}
	}
	if (63 > maxHeightBlock)
		maxHeightBlock = 63;
	//#pragma omp parallel for
	//	for (int y = 5; y < 60; ++y)
	//	{
	//		for (int z = 0; z < 16; ++z)
	//		{
	//			for (int x = 0; x < 16; ++x)
	//			{
	//				 float i = glm::simplex(glm::vec4((x + position.x * 16) / 64.f, (z + position.y * 16) / 64.f, y / 255.f, (float)seed));
	//				if (i > 0.5f)
	//					blocks[y][z][x] = Block(Block::AIR);
	//				//i = glm::perlin(glm::vec3((x + position.x * 16) / 64.f, (z + position.y * 16) / 64.f, y / 127.f), glm::vec3(seed));// + 1.f) * 16.f + 32;
	//				//if (i > 0.0f)
	//				//	blocks[y][z][x] = Block(Block::AIR);
	//
	//				/*if (y > maxHeightBlock)
	//					maxHeightBlock = y;*/
	//
	//					/*for (unsigned char i = 0; i < y; ++i)
	//					{
	//						if (i > 60)
	//							blocks[i][z][x] = Block(Block::GRASS);
	//						else
	//							blocks[i][z][x] = Block(Block::STONE);
	//					}*/
	//			}
	//		}
	//	}
}

bool Chunk::loadChunk(const glm::ivec2& position, const std::filesystem::path& path)
{
	this->position = position;
	saving = false;
	if (std::filesystem::exists(path / std::string(std::to_string(position.x >> 5u) + "." + std::to_string(position.y >> 5u) + ".region")))
	{
		std::ifstream region(path / std::string(std::to_string(position.x >> 5u) + "." + std::to_string(position.y >> 5u) + ".region"), std::ios::binary | std::ios::in);
		if (region.is_open())
		{
			unsigned int offset = ((position.x & 31) + (position.y & 31) * 32);
			region.seekg(offset * 8u, region.beg);
			unsigned int a = 0;
			region.read((char*)&a, 4);
			if (a == 0)
			{
				region.close();
				return false;
			}
			unsigned int size = 0;
			region.read((char*)&size, 4);

			region.seekg(a, region.beg);
			char* buffer = new char[size] {};
			region.read(buffer, size);
			region.read((char*)&maxHeightBlock, sizeof(maxHeightBlock));
			region.close();
			LZ4_decompress_safe(buffer, (char*)&blocks, size, sizeof(blocks));
			modified = true;
			delete[] buffer;
			return true;
		}
	}
	return false;
}

void Chunk::saveChunk(const std::filesystem::path& path)
{
	if (std::filesystem::exists(path / std::string(std::to_string(position.x >> 5u) + "." + std::to_string(position.y >> 5u) + ".region")))
	{
		std::fstream region(path / std::string(std::to_string(position.x >> 5u) + "." + std::to_string(position.y >> 5u) + ".region"), std::ios::binary | std::ios::in | std::ios::out);
		if (region.is_open())
		{
			unsigned int offset = ((position.x & 31) + (position.y & 31) * 32);
			region.seekg(0, region.end);
			unsigned int a = region.tellg();
			region.seekp(offset * 8u, region.beg);
			char* buffer = new char[sizeof(blocks)]{};
			unsigned int size = LZ4_compress_default((char*)&blocks, buffer, sizeof(blocks), sizeof(blocks));

			region.write((char*)&a, 4);
			region.write((char*)&size, 4);
			region.seekp(a, region.beg);
			region.write(buffer, size);
			region.write((char*)&maxHeightBlock, sizeof(maxHeightBlock));
			region.close();
			delete[] buffer;
		}
	}
	else
	{
		std::fstream region(path / std::string(std::to_string(position.x >> 5u) + '.' + std::to_string(position.y >> 5u) + ".region"), std::ios::binary | std::ios::out);
		if (region.is_open())
		{
			unsigned int offset = ((position.x & 31) + (position.y & 31) * 32);
			region.seekp(offset * 8u, region.beg);
			char* buffer = new char[sizeof(blocks)]{};
			unsigned int size = LZ4_compress_default((char*)&blocks, buffer, sizeof(blocks), sizeof(blocks));
			unsigned int a = 8192;
			region.write((char*)&a, 4);
			region.write((char*)&size, 4);
			region.seekp(a, region.beg);
			region.write(buffer, size);
			region.write((char*)&maxHeightBlock, sizeof(maxHeightBlock));
			region.close();
			delete[] buffer;
		}
	}
	saving = false;
}

void Chunk::setBlock(const glm::u8vec3& position, const Block::Type& type)
{
	saving = true;
	modified = true;
	if (position.y > maxHeightBlock)
		maxHeightBlock = position.y;
	blocks[position.y][position.z][position.x] = Block(type);
}

Block& Chunk::getBlock(const glm::u8vec3& position)
{
	return blocks[position.y][position.z][position.x];
}

const glm::ivec2& Chunk::getPosition() const
{
	return position;
}

const unsigned char& Chunk::getMaxHeightBlock() const
{
	return maxHeightBlock;
}

unsigned int Chunk::getCountVertexs()
{
	return countVertex;
}

void Chunk::findNeighbors()
{
	idChunks.clear();
	glm::u8vec2 localPosChunk(glm::abs(chunks[0][0]->position - position));
	glm::u8 a = chunks.size() - 1;
	if (chunks[0][0]->position.y > position.y)
	{
		idChunks.push_back(glm::u8vec2(localPosChunk.x, localPosChunk.y + 1));
	}
	if (position.y < chunks[a][a]->position.y)
	{
		idChunks.push_back(glm::u8vec2(localPosChunk.x, localPosChunk.y - 1));
	}
	if (chunks[0][0]->position.x > position.x)
	{
		idChunks.push_back(glm::u8vec2(localPosChunk.x - 1, localPosChunk.y));
	}
	if (position.x < chunks[a][a]->position.x)
	{
		idChunks.push_back(glm::u8vec2(localPosChunk.x + 1, localPosChunk.y));
	}
}

void Chunk::setModified(const bool& value)
{
	modified = value;
}

std::vector<glm::u8vec2>& Chunk::getNeighbors()
{
	return idChunks;
}

void Chunk::generateMesh()
{
	vao.deleteVAOB();
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> UV;
	std::vector<GLuint> indicies;
	countVertex = 0;
	bool ccubes[256][16][16]{ false };
	cubes.clear();
	for (unsigned char y = 0; y < maxHeightBlock+1; ++y)
	{
		for (unsigned char z = 0; z < 16; ++z)
		{
			for (unsigned char x = 0; x < 16; ++x)
			{
				unsigned short type = blocks[y][z][x].getType();

				if (!ccubes[y][z][x] && isTB({ x, y + 1, z }))
				{
					short icubes = cubes.size() - 1;
					if (cubes.empty())
						cubes.push_back({ x, 1, z, 1, y, 1});
					else if (cubes[icubes].s + cubes[icubes].t - 1 == y && cubes[icubes].x + cubes[icubes].y == x && type == blocks[cubes[icubes].s][cubes[icubes].z][cubes[icubes].x].getType() && cubes[icubes].z + cubes[icubes].w - 1 == z)
						++cubes[icubes].y;
					else
					{
						while (true)
						{
							if (cubes[icubes].s + cubes[icubes].t == 256) break;
							bool f = false;
							while (true)
							{
								if (cmp(ccubes, y, icubes))
								{
									f = true;
									break;
								}
								for (short x = cubes[icubes].x; x < cubes[icubes].x + cubes[icubes].y; ++x)
									ccubes[cubes[icubes].s + cubes[icubes].t][cubes[icubes].z + cubes[icubes].w][x] = true;
								++cubes[icubes].w;
							}
							if (f) break;
							++cubes[icubes].t;
						}
						if (!ccubes[y][z][x] && isTB({ x, y + 1, z }))
							cubes.push_back({ x, 1, z, 1, y, 1 });
					}
				}
				ccubes[y][z][x] = true;
			}
		}
	}
	for (int i = 0; i < cubes.size(); ++i)
	{
		unsigned short type = blocks[cubes[i].s][cubes[i].z][cubes[i].x].getType();
		if (type > 0)
		{
			--type;
			vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(0.f, 1.f, cubes[i].w) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(cubes[i].y, 1.f, cubes[i].w) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(cubes[i].y, 1.f, 0.f) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			normal.push_back({ 0.f, 1.f, 0.f });
			normal.push_back({ 0.f, 1.f, 0.f });
			normal.push_back({ 0.f, 1.f, 0.f });
			normal.push_back({ 0.f, 1.f, 0.f });
			UV.push_back(glm::vec3(0.f, cubes[i].w, type));
			UV.push_back(glm::vec3(0.f, 0.f, type));
			UV.push_back(glm::vec3(cubes[i].y, 0.f, type));
			UV.push_back(glm::vec3(cubes[i].y, cubes[i].w, type));
			indicies.push_back(countVertex);
			indicies.push_back(countVertex + 1);
			indicies.push_back(countVertex + 2);
			indicies.push_back(countVertex + 2);
			indicies.push_back(countVertex + 3);
			indicies.push_back(countVertex);
			countVertex += 4;
			vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(0.f, 0.f, cubes[i].w) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(cubes[i].y, 0.f, cubes[i].w) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			vertex.push_back(glm::vec3(cubes[i].y, 0.f, 0.f) + glm::vec3(cubes[i].x + position.x * 16, cubes[i].s, cubes[i].z + position.y * 16));
			normal.push_back({ 0.f, -1.f, 0.f });
			normal.push_back({ 0.f, -1.f, 0.f });
			normal.push_back({ 0.f, -1.f, 0.f });
			normal.push_back({ 0.f, -1.f, 0.f });
			UV.push_back(glm::vec3(0.f, cubes[i].w, type));
			UV.push_back(glm::vec3(0.f, 0.f, type));
			UV.push_back(glm::vec3(cubes[i].y, 0.f, type));
			UV.push_back(glm::vec3(cubes[i].y, cubes[i].w, type));
			indicies.push_back(countVertex);
			indicies.push_back(countVertex + 1);
			indicies.push_back(countVertex + 2);
			indicies.push_back(countVertex + 2);
			indicies.push_back(countVertex + 3);
			indicies.push_back(countVertex);
			countVertex += 4;
		}
	}
	vao.genVAO();
	vao.addVertexBufferObject(vertex);
	vao.addVertexBufferObject(normal);
	vao.addVertexBufferObject(UV);
	vao.addIndices(indicies);
	modified = false;
}

void Chunk::draw(GL::Program& shader)
{
	if (modified)
	{
		generateMesh();
		vao.draw();
	}
	else
	{
		vao.draw();
	}
}

bool Chunk::isTB(const glm::i16vec3& position)
{
	if (!(position.y >= 0 && position.y < 256))
		return false;
	char chunkPosX = position.x >> 4;
	char chunkPosZ = position.z >> 4;
	if (chunkPosX == 0 && chunkPosZ == 0)
		return blocks[position.y][position.z][position.x].isTransperent();
	unsigned char a = chunks.size() - 1;
	char localPosChunkX = labs(chunks[0][0]->position.x - this->position.x) + chunkPosX;
	char localPosChunkZ = labs(chunks[0][0]->position.y - this->position.y) + chunkPosZ;
	if ((localPosChunkX >= 0 && localPosChunkX <= a) && (localPosChunkZ >= 0 && localPosChunkZ <= a))
	{
		if (chunkPosX == 0 && chunkPosZ != 0)
		{
			return chunks[localPosChunkZ][localPosChunkX]->blocks[position.y][chunkPosZ > 0 ? 0 : 15][position.x].isTransperent();
		}
		else if (chunkPosX != 0 && chunkPosZ == 0)
		{
			return chunks[localPosChunkZ][localPosChunkX]->blocks[position.y][position.z][chunkPosX > 0 ? 0 : 15].isTransperent();
		}
	}
	return false;
}

bool Chunk::cmp(bool ccubes[256][16][16], const unsigned char& y, const unsigned short& icubes)
{
	if (cubes[icubes].z + cubes[icubes].w == 16) return false;
	unsigned char width = cubes[icubes].x + cubes[icubes].y;
	unsigned char hight = cubes[icubes].z + cubes[icubes].w;
	unsigned char depth = cubes[icubes].s + cubes[icubes].t;
	unsigned short type = blocks[cubes[icubes].s][cubes[icubes].z][cubes[icubes].x].getType();
	for (short x = cubes[icubes].x; x < width; ++x)
	{
		if (!ccubes[depth][hight][x] &&
			isTB({ x, depth, hight }) &&
			type == blocks[depth][hight][x].getType())
			continue;
		else
			return true;
	}
	return false;
}
