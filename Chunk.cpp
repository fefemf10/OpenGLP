#include "Chunk.hpp"

Chunk::Chunk(std::vector<std::vector<Chunk*>>& chunks) : blocks{ Block::AIR }, chunks(chunks), maxHeightBlock{ 255 }, countTriangle(), countVertex(), modified()
{

}

Chunk::~Chunk()
{
}

void Chunk::generateChunk(const glm::ivec2& position, const double& seed)
{
	this->position = position;
	modified = true;
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
				return false;
			unsigned int size = 0;
			region.read((char*)&size, 4);

			region.seekg(a, region.beg);
			char* buffer = new char[size] {};
			region.read(buffer, size);
			region.read((char*)&maxHeightBlock, sizeof(maxHeightBlock));
			LZ4_decompress_safe(buffer, (char*)&blocks, size, sizeof(blocks));
			region.close();
			modified = true;
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
		}
	}
}

void Chunk::setBlock(const glm::u8vec3& position, const Block::Type& type)
{
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

unsigned int Chunk::getCountTriangles()
{
	return countTriangle;
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
	std::vector<glm::vec2> UV;
	std::vector<GLuint> indicies;
	countVertex = 0;
	countTriangle = 0;
	for (int y = 0; y < maxHeightBlock; ++y)
	{
		for (int z = 0; z < 16; ++z)
		{
			std::vector<glm::ivec3> sup;
			std::vector<glm::ivec3> sdown;
			std::vector<glm::ivec3> sleft;
			std::vector<glm::ivec3> sright;
			std::vector<glm::ivec3> sfront;
			std::vector<glm::ivec3> sback;
			std::vector<glm::ivec3> scenter;
			int x = 0;
			//tblocks.insert(tblocks.end(), { checkBlock({ x, y + 1, z }), checkBlock({ x, y - 1, z }), checkBlock({ x - 1, y, z }), checkBlock({ x + 1, y, z }), checkBlock({ x, y, z + 1 }), checkBlock({ x, y, z - 1 }) });
			for (int x = 0; x < 16; ++x)
			{
				if (blocks[y][z][x].getType() != Block::AIR)
				{
					if (isTB({ x, y + 1, z }))
						if (sup.empty())
							sup.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sup[sup.size() - 1].x + sup[sup.size() - 1].y == x && sup[sup.size() - 1].z == blocks[y][z][x].getType())
							++sup[sup.size() - 1].y;
						else sup.push_back({ x, 1, blocks[y][z][x].getType() });
					if (isTB({ x, y - 1, z }))
						if (sdown.empty())
							sdown.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sdown[sdown.size() - 1].x + sdown[sdown.size() - 1].y == x && sdown[sdown.size() - 1].z == blocks[y][z][x].getType())
							++sdown[sdown.size() - 1].y;
						else sdown.push_back({ x, 1, blocks[y][z][x].getType() });

					if (isTB({ x - 1, y, z }))
						if (sleft.empty())
							sleft.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sleft[sleft.size() - 1].x + sleft[sleft.size() - 1].y == x && sleft[sleft.size() - 1].z == blocks[y][z][x].getType())
							++sleft[sleft.size() - 1].y;
						else sleft.push_back({ x, 1, blocks[y][z][x].getType() });
					if (isTB({ x + 1, y, z }))
						if (sright.empty())
							sright.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sright[sright.size() - 1].x + sright[sright.size() - 1].y == x && sright[sright.size() - 1].z == blocks[y][z][x].getType())
							++sright[sright.size() - 1].y;
						else sright.push_back({ x, 1, blocks[y][z][x].getType() });

					if (isTB({ x, y, z + 1}))
						if (sfront.empty())
							sfront.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sfront[sfront.size() - 1].x + sfront[sfront.size() - 1].y == x && sfront[sfront.size() - 1].z == blocks[y][z][x].getType())
							++sfront[sfront.size() - 1].y;
						else sfront.push_back({ x, 1, blocks[y][z][x].getType() });
					if (isTB({ x, y, z - 1}))
						if (sback.empty())
							sback.push_back({ x, 1, blocks[y][z][x].getType() });
						else if (sback[sback.size() - 1].x + sback[sback.size() - 1].y == x && sback[sback.size() - 1].z == blocks[y][z][x].getType())
							++sback[sback.size() - 1].y;
						else sback.push_back({ x, 1, blocks[y][z][x].getType() });
				}
			}
			for (int i = 0; i < sup.size(); i++)
			{
				vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(sup[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 1.f, 1.f) + glm::vec3(sup[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sup[i].y, 1.f, 1.f) + glm::vec3(sup[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sup[i].y, 1.f, 0.f) + glm::vec3(sup[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				float uvSize = 1.f / 5.f;
				float u = ((sup[i].z - 1) % 5) * uvSize;
				float v = ((sup[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			for (int i = 0; i < sdown.size(); i++)
			{
				vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(sdown[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, 1.f) + glm::vec3(sdown[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sdown[i].y, 0.f, 1.f) + glm::vec3(sdown[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sdown[i].y, 0.f, 0.f) + glm::vec3(sdown[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				float uvSize = 1.f / 5.f;
				float u = ((sdown[i].z - 1) % 5) * uvSize;
				float v = ((sdown[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			for (int i = 0; i < sleft.size(); i++)
			{
				vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(sleft[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(sleft[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, 1.f) + glm::vec3(sleft[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 1.f, 1.f) + glm::vec3(sleft[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				float uvSize = 1.f / 5.f;
				float u = ((sleft[i].z - 1) % 5) * uvSize;
				float v = ((sleft[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			for (int i = 0; i < sright.size(); i++)
			{
				vertex.push_back(glm::vec3(1.f, 1.f, 0.f) + glm::vec3(sright[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(1.f, 0.f, 0.f) + glm::vec3(sright[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(1.f, 0.f, 1.f) + glm::vec3(sright[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(1.f, 1.f, 1.f) + glm::vec3(sright[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				float uvSize = 1.f / 5.f;
				float u = ((sright[i].z - 1) % 5) * uvSize;
				float v = ((sright[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			for (int i = 0; i < sfront.size(); i++)
			{
				vertex.push_back(glm::vec3(0.f, 1.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sfront[i].y, 0.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sfront[i].y, 1.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
				float uvSize = 1.f / 5.f;
				float u = ((sfront[i].z - 1) % 5) * uvSize;
				float v = ((sfront[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			for (int i = 0; i < sback.size(); i++)
			{
				vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sback[i].y, 0.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
				vertex.push_back(glm::vec3(sback[i].y, 1.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
				float uvSize = 1.f / 5.f;
				float u = ((sback[i].z - 1) % 5) * uvSize;
				float v = ((sback[i].z - 1) / 5) * uvSize;
				UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
				UV.push_back(glm::vec2(u, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
				UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
				countTriangle += 2;
			}
			//{
			//	unsigned short type = blocks[y][z][x].getType();
			//	glm::vec3 v[8] = {
			//		glm::vec3(0.f, 1.f, 1.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(0.f, 0.f, 1.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(1.f, 0.f, 1.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(1.f, 1.f, 1.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(0.f, 1.f, 0.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(0.f, 0.f, 0.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(1.f, 0.f, 0.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16),
			//		glm::vec3(1.f, 1.f, 0.f) + glm::vec3(x + position.x * 16, y, z + position.y * 16)
			//	};
			//	for (unsigned char i = 0; i < 6; i++)
			//	{
			//		if (tblocks[i].getProperties() & Block::TRANSPERENT)
			//		{
			//			switch (i)
			//			{
			//			case 0://UP
			//				vertex.push_back(v[4]);
			//				vertex.push_back(v[0]);
			//				vertex.push_back(v[3]);
			//				vertex.push_back(v[7]);
			//				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
			//				break;
			//			case 1://DOWN
			//				vertex.push_back(v[5]);
			//				vertex.push_back(v[1]);
			//				vertex.push_back(v[2]);
			//				vertex.push_back(v[6]);
			//				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
			//				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
			//				break;
			//			case 2://LEFT
			//				vertex.push_back(v[4]);
			//				vertex.push_back(v[5]);
			//				vertex.push_back(v[1]);
			//				vertex.push_back(v[0]);
			//				normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
			//				break;
			//			case 3://RIGHT
			//				vertex.push_back(v[7]);
			//				vertex.push_back(v[6]);
			//				vertex.push_back(v[2]);
			//				vertex.push_back(v[3]);
			//				normal.push_back(glm::vec3(1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(1.f, 0.f, 0.f));
			//				normal.push_back(glm::vec3(1.f, 0.f, 0.f));
			//				break;
			//			case 4://FRONT
			//				vertex.push_back(v[0]);
			//				vertex.push_back(v[1]);
			//				vertex.push_back(v[2]);
			//				vertex.push_back(v[3]);
			//				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, 1.f));
			//				break;
			//			case 5://BACK
			//				vertex.push_back(v[4]);
			//				vertex.push_back(v[5]);
			//				vertex.push_back(v[6]);
			//				vertex.push_back(v[7]);
			//				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
			//				normal.push_back(glm::vec3(0.f, 0.f, -1.f));
			//				break;
			//			}
			//			float uvSize = 1.f / 5.f;
			//			float u = ((type - 1) % 5) * uvSize;
			//			float v = ((type - 1) / 5) * uvSize;
			//			UV.push_back(glm::vec2(u, v + uvSize) + 1.f);
			//			UV.push_back(glm::vec2(u, v) + 1.f);
			//			UV.push_back(glm::vec2(u + uvSize, v) + 1.f);
			//			UV.push_back(glm::vec2(u + uvSize, v + uvSize) + 1.f);
			/*			indicies.push_back(countVertex);
						indicies.push_back(countVertex + 1);
						indicies.push_back(countVertex + 2);
						indicies.push_back(countVertex + 2);
						indicies.push_back(countVertex + 3);
						indicies.push_back(countVertex);
						countVertex += 4;
						countTriangle += 2;*/
						//		}
						//	}
						//}
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
