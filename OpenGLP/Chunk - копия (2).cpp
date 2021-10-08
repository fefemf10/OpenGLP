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
			unsigned short y = static_cast<unsigned char>((glm::simplex(glm::vec3((x + position.x * 16) * 0.00625f, (float)seed, (z + position.y * 16) * 0.00625f)) + 1.f) * 128.f);
			unsigned short y1 = static_cast<unsigned char>((glm::simplex(glm::vec3((x + position.x * 16) * 0.0125f, (float)seed, (z + position.y * 16) * 0.0125f)) + 1.f) * 64.f);
			unsigned short y2 = static_cast<unsigned char>((glm::simplex(glm::vec3((x + position.x * 16) * 0.025f, (float)seed, (z + position.y * 16) * 0.025f)) + 1.f) * 32.f);
			unsigned short y3 = static_cast<unsigned char>((glm::simplex(glm::vec3((x + position.x * 16) * 0.05f, (float)seed, (z + position.y * 16) * 0.05f)) + 1.f) * 16.f);
			y = (y + y1 + y2 + y3) / 4;
			for (unsigned char i = 1; i < y; ++i)
			{
				blocks[i][z][x] = Block(Block::STONE);
			}
			unsigned char dirt = rand()%4;
			for (unsigned char i = 1; i < y-dirt; ++i)
			{
				blocks[i][z][x] = Block(Block::STONE);
				float p = glm::simplex(glm::vec4((x + position.x * 16) * 0.0125f, i * 0.0125f, (z + position.y * 16) * 0.0125f, (float)seed));
				float p1 = glm::simplex(glm::vec4((x + position.x * 16) * 0.025f, i * 0.025f, (z + position.y * 16) * 0.025f, (float)seed));
				if ((p + p1) / 2 < 0.f)
					blocks[i][z][x] = Block(Block::Type::AIR);
			}
			for (unsigned char i = y-dirt; i < y; ++i)
			{
				blocks[i][z][x] = Block(Block::DIRT);
				float p = glm::simplex(glm::vec4((x + position.x * 16) * 0.0125f, i * 0.0125f, (z + position.y * 16) * 0.0125f, (float)seed));
				float p1 = glm::simplex(glm::vec4((x + position.x * 16) * 0.025f, i * 0.025f, (z + position.y * 16) * 0.025f, (float)seed));
				if ((p + p1)/2 < 0.f)
					blocks[i][z][x] = Block(Block::Type::AIR);
			}
			if (y > maxHeightBlock)
				maxHeightBlock = y;
		}

//#pragma omp parallel for
//	for (int y = 1; y < 64; ++y)
//	{
//		for (int z = 0; z < 16; ++z)
//		{
//			for (int x = 0; x < 16; ++x)
//			{
//				float p = glm::simplex(glm::vec4((x + position.x * 16) * 0.0125f, y * 0.0125f, (z + position.y * 16) * 0.0125f, (float)seed));
//				if (p < 0.f)
//					blocks[y][z][x] = Block(Block::Type::AIR);
//			}
//		}
//	}
//	if (63 > maxHeightBlock)
//		maxHeightBlock = 63;
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
	for (short y = 0; y < maxHeightBlock+1; ++y)
	{
		bool cup[16][16]{ false };
		bool cdown[16][16]{ false };
		std::vector<glm::u8vec4> sup;
		std::vector<glm::u8vec4> sdown;
		for (short z = 0; z < 16; ++z)
		{
			std::vector<glm::u8vec2> sleft;
			std::vector<glm::u8vec2> sright;
			std::vector<glm::u8vec2> sfront;
			std::vector<glm::u8vec2> sback;
			for (short x = 0; x < 16; ++x)
			{
				unsigned short type = blocks[y][z][x].getType();
				unsigned short type2 = blocks[y][x][z].getType();

				if (!cup[z][x] && isTB({ x, y + 1, z }))
				{
					short isup = sup.size() - 1;
					if (sup.empty())
						sup.push_back({ x, 1, z, 1 });
					else if (sup[isup].x + sup[isup].y == x && type == blocks[y][sup[isup].z][sup[isup].x].getType() && sup[isup].z + sup[isup].w - 1 == z)
						++sup[isup].y;
					else
					{
						while (true)
						{
							if (sup[isup].z + sup[isup].w == 16) break;
							bool f = false;
							for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
							{
								if (!cup[sup[isup].z + sup[isup].w][x] && isTB({ x, y + 1, sup[isup].z + sup[isup].w }) && blocks[y][sup[isup].z][sup[isup].x].getType() == blocks[y][sup[isup].z + sup[isup].w][x].getType())
									continue;
								else
								{
									f = true;
									break;
								}
							}
							if (f) break;
							for (short x = sup[isup].x; x < sup[isup].x + sup[isup].y; ++x)
								cup[sup[isup].z + sup[isup].w][x] = true;
							++sup[isup].w;
						}
						if (!cup[z][x] && isTB({ x, y + 1, z }))
							sup.push_back({ x, 1, z, 1 });
					}
				}
				cup[z][x] = true;

				if (!cdown[z][x] && isTB({ x, y - 1, z }))
				{
					short isdown = sdown.size() - 1;
					if (sdown.empty())
						sdown.push_back({ x, 1, z, 1 });
					else if (sdown[isdown].x + sdown[isdown].y == x && type == blocks[y][sdown[isdown].z][sdown[isdown].x].getType() && sdown[isdown].z + sdown[isdown].w - 1 == z)
						++sdown[isdown].y;
					else
					{
						while (true)
						{
							if (sdown[isdown].z + sdown[isdown].w == 16) break;
							bool f = false;
							for (short x = sdown[isdown].x; x < sdown[isdown].x + sdown[isdown].y; ++x)
							{
								if (!cdown[sdown[isdown].z + sdown[isdown].w][x] && isTB({ x, y - 1, sdown[isdown].z + sdown[isdown].w }) && blocks[y][sdown[isdown].z][sdown[isdown].x].getType() == blocks[y][sdown[isdown].z + sdown[isdown].w][x].getType())
									continue;
								else
								{
									f = true;
									break;
								}
							}
							if (f) break;
							for (short x = sdown[isdown].x; x < sdown[isdown].x + sdown[isdown].y; ++x)
								cdown[sdown[isdown].z + sdown[isdown].w][x] = true;
							++sdown[isdown].w;
						}
						if (!cdown[z][x] && isTB({ x, y - 1, z }))
							sdown.push_back({ x, 1, z, 1 });
					}
				}
				cdown[z][x] = true;
				unsigned char isfront = sfront.size() - 1;
				unsigned char isback = sback.size() - 1;
				if (isTB({ x, y, z + 1 }))
					if (sfront.empty())
						sfront.push_back({ x, 1 });
					else if (sfront[isfront].x + sfront[isfront].y == x && blocks[y][z][sfront[isfront].x].getType() == type)
						++sfront[isfront].y;
					else sfront.push_back({ x, 1 });
				if (isTB({ x, y, z - 1 }))
					if (sback.empty())
						sback.push_back({ x, 1 });
					else if (sback[isback].x + sback[isback].y == x && blocks[y][z][sback[isback].x].getType() == type)
						++sback[isback].y;
					else sback.push_back({ x, 1 });

				unsigned char isleft = sleft.size() - 1;
				unsigned char isright = sright.size() - 1;
				if (isTB({ z - 1, y, x }))
					if (sleft.empty())
						sleft.push_back({ x, 1 });
					else if (sleft[isleft].x + sleft[isleft].y == x && blocks[y][sleft[isleft].x][z].getType() == type2)
						++sleft[isleft].y;
					else sleft.push_back({ x, 1 });
				if (isTB({ z + 1, y, x }))
					if (sright.empty())
						sright.push_back({ x, 1 });
					else if (sright[isright].x + sright[isright].y == x && blocks[y][sright[isright].x][z].getType() == type2)
						++sright[isright].y;
					else sright.push_back({ x, 1 });
			}
			for (int i = 0; i < sleft.size(); ++i)
			{
				unsigned short type = blocks[y][sleft[i].x][z].getType();
				if (type > 0)
				{
					--type;
					vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(z + position.x * 16, y, sleft[i].x + position.y * 16));
					vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(z + position.x * 16, y, sleft[i].x + position.y * 16));
					vertex.push_back(glm::vec3(0.f, 0.f, sleft[i].y) + glm::vec3(z + position.x * 16, y, sleft[i].x + position.y * 16));
					vertex.push_back(glm::vec3(0.f, 1.f, sleft[i].y) + glm::vec3(z + position.x * 16, y, sleft[i].x + position.y * 16));
					normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(-1.f, 0.f, 0.f));
					UV.push_back(glm::vec3(0.f, 1.f, type));
					UV.push_back(glm::vec3(0.f, 0.f, type));
					UV.push_back(glm::vec3(sleft[i].y, 0.f, type));
					UV.push_back(glm::vec3(sleft[i].y, 1.f, type));
					indicies.push_back(countVertex);
					indicies.push_back(countVertex + 1);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 3);
					indicies.push_back(countVertex);
					countVertex += 4;
				}
			}
			for (int i = 0; i < sright.size(); ++i)
			{
				unsigned short type = blocks[y][sright[i].x][z].getType();
				if (type > 0)
				{
					--type;
					vertex.push_back(glm::vec3(1.f, 1.f, 0.f) + glm::vec3(z + position.x * 16, y, sright[i].x + position.y * 16));
					vertex.push_back(glm::vec3(1.f, 0.f, 0.f) + glm::vec3(z + position.x * 16, y, sright[i].x + position.y * 16));
					vertex.push_back(glm::vec3(1.f, 0.f, sright[i].y) + glm::vec3(z + position.x * 16, y, sright[i].x + position.y * 16));
					vertex.push_back(glm::vec3(1.f, 1.f, sright[i].y) + glm::vec3(z + position.x * 16, y, sright[i].x + position.y * 16));
					normal.push_back(glm::vec3(1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(1.f, 0.f, 0.f));
					normal.push_back(glm::vec3(1.f, 0.f, 0.f));
					UV.push_back(glm::vec3(0.f, 1.f, type));
					UV.push_back(glm::vec3(0.f, 0.f, type));
					UV.push_back(glm::vec3(sright[i].y, 0.f, type));
					UV.push_back(glm::vec3(sright[i].y, 1.f, type));
					indicies.push_back(countVertex);
					indicies.push_back(countVertex + 1);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 3);
					indicies.push_back(countVertex);
					countVertex += 4;
				}
			}
			for (int i = 0; i < sfront.size(); ++i)
			{
				unsigned short type = blocks[y][z][sfront[i].x].getType();
				if (type > 0)
				{
					--type;
					vertex.push_back(glm::vec3(0.f, 1.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(0.f, 0.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(sfront[i].y, 0.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(sfront[i].y, 1.f, 1.f) + glm::vec3(sfront[i].x + position.x * 16, y, z + position.y * 16));
					normal.push_back(glm::vec3(0.f, 0.f, 1.f));
					normal.push_back(glm::vec3(0.f, 0.f, 1.f));
					normal.push_back(glm::vec3(0.f, 0.f, 1.f));
					normal.push_back(glm::vec3(0.f, 0.f, 1.f));
					UV.push_back(glm::vec3(0.f, 1.f, type));
					UV.push_back(glm::vec3(0.f, 0.f, type));
					UV.push_back(glm::vec3(sfront[i].y, 0.f, type));
					UV.push_back(glm::vec3(sfront[i].y, 1.f, type));
					indicies.push_back(countVertex);
					indicies.push_back(countVertex + 1);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 3);
					indicies.push_back(countVertex);
					countVertex += 4;
				}
			}
			for (int i = 0; i < sback.size(); ++i)
			{
				unsigned short type = blocks[y][z][sback[i].x].getType();
				if (type > 0)
				{
					--type;
					vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(sback[i].y, 0.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
					vertex.push_back(glm::vec3(sback[i].y, 1.f, 0.f) + glm::vec3(sback[i].x + position.x * 16, y, z + position.y * 16));
					normal.push_back(glm::vec3(0.f, 0.f, -1.f));
					normal.push_back(glm::vec3(0.f, 0.f, -1.f));
					normal.push_back(glm::vec3(0.f, 0.f, -1.f));
					normal.push_back(glm::vec3(0.f, 0.f, -1.f));
					UV.push_back(glm::vec3(0.f, 1.f, type));
					UV.push_back(glm::vec3(0.f, 0.f, type));
					UV.push_back(glm::vec3(sback[i].y, 0.f, type));
					UV.push_back(glm::vec3(sback[i].y, 1.f, type));
					indicies.push_back(countVertex);
					indicies.push_back(countVertex + 1);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 2);
					indicies.push_back(countVertex + 3);
					indicies.push_back(countVertex);
					countVertex += 4;
				}
			}
		}
		for (int i = 0; i < sup.size(); ++i)
		{
			unsigned short type = blocks[y][sup[i].z][sup[i].x].getType();
			if (type > 0)
			{
				--type;
				vertex.push_back(glm::vec3(0.f, 1.f, 0.f) + glm::vec3(sup[i].x + position.x * 16, y, sup[i].z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 1.f, sup[i].w) + glm::vec3(sup[i].x + position.x * 16, y, sup[i].z + position.y * 16));
				vertex.push_back(glm::vec3(sup[i].y, 1.f, sup[i].w) + glm::vec3(sup[i].x + position.x * 16, y, sup[i].z + position.y * 16));
				vertex.push_back(glm::vec3(sup[i].y, 1.f, 0.f) + glm::vec3(sup[i].x + position.x * 16, y, sup[i].z + position.y * 16));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				normal.push_back(glm::vec3(0.f, 1.f, 0.f));
				UV.push_back(glm::vec3(0.f, sup[i].w, type));
				UV.push_back(glm::vec3(0.f, 0.f, type));
				UV.push_back(glm::vec3(sup[i].y, 0.f, type));
				UV.push_back(glm::vec3(sup[i].y, sup[i].w, type));
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
			}
		}
		for (int i = 0; i < sdown.size(); ++i)
		{
			unsigned short type = blocks[y][sdown[i].z][sdown[i].x].getType();
			if (type > 0)
			{
				--type;
				vertex.push_back(glm::vec3(0.f, 0.f, 0.f) + glm::vec3(sdown[i].x + position.x * 16, y, sdown[i].z + position.y * 16));
				vertex.push_back(glm::vec3(0.f, 0.f, sdown[i].w) + glm::vec3(sdown[i].x + position.x * 16, y, sdown[i].z + position.y * 16));
				vertex.push_back(glm::vec3(sdown[i].y, 0.f, sdown[i].w) + glm::vec3(sdown[i].x + position.x * 16, y, sdown[i].z + position.y * 16));
				vertex.push_back(glm::vec3(sdown[i].y, 0.f, 0.f) + glm::vec3(sdown[i].x + position.x * 16, y, sdown[i].z + position.y * 16));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				normal.push_back(glm::vec3(0.f, -1.f, 0.f));
				UV.push_back(glm::vec3(0.f, sdown[i].w, type));
				UV.push_back(glm::vec3(0.f, 0.f, type));
				UV.push_back(glm::vec3(sdown[i].y, 0.f, type));
				UV.push_back(glm::vec3(sdown[i].y, sdown[i].w, type));
				indicies.push_back(countVertex);
				indicies.push_back(countVertex + 1);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 2);
				indicies.push_back(countVertex + 3);
				indicies.push_back(countVertex);
				countVertex += 4;
			}
		}
	}
	for (unsigned char z = 0; z < 16; ++z)
	{

		for (unsigned char y = 0; y < maxHeightBlock + 1; ++y)
			for (unsigned char x = 0; x < 16; ++x)
			{

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
