#pragma once
#include "Enums.hpp"
struct PaletteItem
{
	Enums::Block id{};
	std::map<Enums::PropertiesBlock, uint8_t> properties;
};