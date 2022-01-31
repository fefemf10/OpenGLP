#include "Chunk.hpp"

Chunk::Chunk(std::vector<std::vector<Chunk*>>& chunks) : chunks(chunks)
{
}

void Chunk::genMesh()
{
	modified.store(false);
	work.store(true);
	for (size_t i = 0; i < sections.size(); ++i)
	{
		if (!sections[i].blockPalette.empty())
		{
			if (!(sections[i].blockPalette.size() == 1 && sections[i].blockPalette[0].id == Enums::Block::AIR) && !sections[i].work.load())
			{
				sections[i].genMesh();
			}
		}
	}
	work.store(false);
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