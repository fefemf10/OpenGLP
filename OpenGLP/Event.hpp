#pragma once
#include "GLM/vec2.hpp"

enum class Events
{
	ChunkNeedLoadEvent,
	ChunkNeedFloodfillEvent,
	ChunkNeedMeshEvent,
	ChunkNeedBufferEvent,
	ChunkLoadedEvent,
	ChunkFloodfilledEvent,
	ChunkMeshedEvent
};

class Event
{
public:
	Event() = default;
	virtual ~Event() = default;
	virtual Events type() const;
protected:
	Events event{};
};

class ChunkEvent : public Event
{
public:
	ChunkEvent(const glm::ivec2& position) : position(position) {};
	glm::ivec2 position{};
};

class ChunkNeedLoadEvent : public ChunkEvent
{
public:
	ChunkNeedLoadEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkNeedLoadEvent; };
};

class ChunkNeedFloodfillEvent : public ChunkEvent
{
public:
	ChunkNeedFloodfillEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkNeedFloodfillEvent; };
};

class ChunkNeedMeshEvent : public ChunkEvent
{
public:
	ChunkNeedMeshEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkNeedMeshEvent; };
};

class ChunkNeedBufferEvent : public ChunkEvent
{
public:
	ChunkNeedBufferEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkNeedBufferEvent; };
};

class ChunkLoadedEvent : public ChunkEvent
{
public:
	ChunkLoadedEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkLoadedEvent; };
};

class ChunkFloodfilledEvent : public ChunkEvent
{
public:
	ChunkFloodfilledEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkFloodfilledEvent; };
};

class ChunkMeshedEvent : public ChunkEvent
{
public:
	ChunkMeshedEvent(const glm::ivec2& position) : ChunkEvent(position) { event = Events::ChunkMeshedEvent; };
};