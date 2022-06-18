#include "Chunk.hpp"

void Chunk::genMesh()
{
	for (auto& section : sections)
		if (!section.hasOnlyAir())
			section.genMesh();
}

Section* Chunk::getSection(int32_t position)
{
	for (auto& section : sections)
		if (section.position.y == position)
			return &section;
	return nullptr;
}

bool Chunk::isModified() const
{
	for (auto& section : sections)
		if (section.modified.load())
			return true;
	return false;
}

bool Chunk::isWork() const
{
	for (auto& section : sections)
		if (section.work.load())
			return true;
	return false;
}

void Chunk::setModified(bool value)
{
	for (auto& section : sections)
	{
		section.modified.store(value);
		section.modified.notify_all();
	}
}

void Chunk::setWork(bool value)
{
	for (auto& section : sections)
		section.work.store(value);
	work.store(value);
	work.notify_all();
}
