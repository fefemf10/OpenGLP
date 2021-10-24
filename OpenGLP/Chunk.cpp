#include "Chunk.hpp"
#include "Block.hpp"

Chunk::Chunk(std::vector<std::vector<Chunk*>>& chunks) : chunks(chunks)
{
}

void Chunk::genMesh()
{
	modified = false;
	for (size_t i = 0; i < sections.size(); ++i)
	{
		if (!sections[i].blockPalette.empty())
		{
			if (!(sections[i].blockPalette.size() == 1 && sections[i].blockPalette[0].id == Enums::Block::AIR))
			{
				sections[i].genMesh();
			}
		}
	}
}

void Chunk::draw(const Frustum& frustum)
{
	countVertex = 0;
	countVisibleSection = 0;
	for (size_t i = 0; i < sections.size(); ++i)
	{
		const unsigned char f = frustum.CubeInFrustum(glm::vec3(position.x * 16 + 8, sections[i].position.y * 16 + 8, position.y * 16 + 8), 8, 8, 8);
		if (f == Frustum::FRUSTUM_INTERSECT || f == Frustum::FRUSTUM_INSIDE)
		{
			sections[i].draw();
			countVertex += sections[i].countVertex;
			if (sections[i].visible)
				++countVisibleSection;
		}
	}
}

void Chunk::drawTransperent(const Frustum& frustum)
{
	countVertexTransperent = 0;
	for (size_t i = 0; i < sections.size(); ++i)
	{
		const unsigned char f = frustum.CubeInFrustum(glm::vec3(position.x * 16 + 8, sections[i].position.y * 16 + 8, position.y * 16 + 8), 8, 8, 8);
		if (f == Frustum::FRUSTUM_INTERSECT || f == Frustum::FRUSTUM_INSIDE)
		{
			sections[i].drawTransperent();
			countVertexTransperent += sections[i].countVertexTransperent;
		}
	}
}

void Chunk::findNeighbors()
{
	//for (long long i = 1; i < static_cast<int>(sections.size())-1; ++i)
	//{
	//	/*if (i == 0)
	//	{
	//		sections[0].neighbors[0] = nullptr;
	//		sections[0].neighbors[5] = (sections.size() >= 2) ? &sections[1] : nullptr;
	//	}
	//	else if (i == sections.size() - 1)
	//	{
	//		sections[sections.size() - 1].neighbors[5] = nullptr;
	//		sections[sections.size() - 1].neighbors[0] = (sections.size() - 2 < 0) ? &sections[sections.size() - 2] : nullptr;
	//	}*/
	//	sections[i].neighbors[0] = &sections[i - 1];
	//	sections[i].neighbors[5] = &sections[i + 1];
	//	if (localPosition.y - 1 >= 0 && chunks[localPosition.y - 1][localPosition.x]->sections.size() > i)
	//		sections[i].neighbors[1] = &chunks[localPosition.y - 1][localPosition.x]->sections[i];
	//	else
	//		sections[i].neighbors[1] = nullptr;
	//	if (localPosition.x - 1 >= 0 && chunks[localPosition.y][localPosition.x - 1]->sections.size() > i)
	//		sections[i].neighbors[2] = &chunks[localPosition.y][localPosition.x - 1]->sections[i];
	//	else
	//		sections[i].neighbors[2] = nullptr;
	//	if (localPosition.x + 1 < chunks.size() && chunks[localPosition.y][localPosition.x + 1]->sections.size() > i)
	//		sections[i].neighbors[3] = &chunks[localPosition.y][localPosition.x + 1]->sections[i];
	//	else
	//		sections[i].neighbors[3] = nullptr;
	//	if (localPosition.y + 1 < chunks.size() && chunks[localPosition.y + 1][localPosition.x]->sections.size() > i)
	//		sections[i].neighbors[4] = &chunks[localPosition.y + 1][localPosition.x]->sections[i];
	//	else
	//		sections[i].neighbors[4] = nullptr;
	//}
}

Section* Chunk::getSection(int32_t position)
{
	for (size_t i = 0; i < sections.size(); ++i)
	{
		if (sections[i].position.y == position)
			return &sections[i];
	}
	return nullptr;
}

//unsigned char Chunk::noise(unsigned char countOctaves, double x, double y, double z, double persistence, double scale, unsigned char min, unsigned char max)
//{
//	double maxAmp = 0.0;
//	double amp = 1.0;
//	double freq = scale;
//	double noise{};
//
//	for (unsigned char i = 0; i < countOctaves; ++i)
//	{
//		noise += glm::simplex(glm::dvec3(x * freq, y, z * freq)) * amp;
//		maxAmp += amp;
//		amp *= persistence;
//		freq *= 2.0;
//	}
//	noise /= maxAmp;
//	return glm::clamp((std::pow((noise), 0.5)) * 128.0, (double)min, (double)max);
//}