#pragma once
#include <string>
#include <map>
#include <set>
#include "TextureSlot.hpp"

class TextureMapping
{
public:
	std::map<TextureSlot, std::string> slots;
	std::set<TextureSlot> forcedSlots;
};