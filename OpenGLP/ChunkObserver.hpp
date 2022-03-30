#pragma once
#include "Observer.hpp"

class World;

class ChunkObserver : public Observer
{
public:
	ChunkObserver(World& world);
	void handle(const Event& event) override;
private:
	World& world;
};

