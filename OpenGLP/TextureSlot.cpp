#include "TextureSlot.hpp"

const TextureSlot TextureSlot::NONE{ .id="", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::ALL{ .id="all", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::TEXTURE{ .id="texture", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::PARTICLE{ .id="particle", .parent = TextureSlot::TEXTURE};
const TextureSlot TextureSlot::END{ .id="end", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::BOTTOM{ .id="bottom", .parent = TextureSlot::END};
const TextureSlot TextureSlot::TOP{ .id="top", .parent = TextureSlot::END};
const TextureSlot TextureSlot::FRONT{ .id="front", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::BACK{ .id="back", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::SIDE{ .id="side", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::NORTH{ .id="north", .parent = TextureSlot::SIDE };
const TextureSlot TextureSlot::SOUTH{ .id="south", .parent = TextureSlot::SIDE };
const TextureSlot TextureSlot::EAST{ .id="east", .parent = TextureSlot::SIDE };
const TextureSlot TextureSlot::WEST{ .id="west", .parent = TextureSlot::SIDE};
const TextureSlot TextureSlot::UP{ .id="up", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::DOWN{ .id="down", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::CROSS{ .id="cross", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::PLANT{ .id="plant", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::WALL{ .id="wall", .parent = TextureSlot::ALL};
const TextureSlot TextureSlot::RAIL{ .id="rail", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::WOOL{ .id="wool", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::PATTERN{ .id="pattern", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::PANE{ .id="pane", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::EDGE{ .id="edge", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::FAN{ .id="fan", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::STEM{ .id="stem", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::UPPER_STEM{ .id="upper_stem", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::CROP{ .id="crop", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::DIRT{ .id="dirt", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::FIRE{ .id="fire", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::LANTERN{ .id="lantern", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::PLATFORM{ .id="platform", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::UNSTICKY{ .id="unsticky", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::TORCH{ .id="torth", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::LAYER0{ .id="layer0", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::LIT_LOG{ .id="lit_fog", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::CANDLE{ .id="candle", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::INSIDE{ .id="inside", .parent = TextureSlot::NONE};
const TextureSlot TextureSlot::CONTENT{ .id="content", .parent = TextureSlot::NONE};

TextureSlot TextureSlot::getTextureSlot(const std::string& id) noexcept
{
	if (id == "#all")
		return TextureSlot::ALL;
	else if (id == "#texture")
		return TextureSlot::TEXTURE;
	else if (id == "#particle")
		return TextureSlot::PARTICLE;
	else if (id == "#end")
		return TextureSlot::END;
	else if (id == "#bottom")
		return TextureSlot::BOTTOM;
	else if (id == "#top")
		return TextureSlot::TOP;
	else if (id == "#front")
		return TextureSlot::FRONT;
	else if (id == "#back")
		return TextureSlot::BACK;
	else if (id == "#side")
		return TextureSlot::SIDE;
	else if (id == "#north")
		return TextureSlot::NORTH;
	else if (id == "#south")
		return TextureSlot::SOUTH;
	else if (id == "#east")
		return TextureSlot::EAST;
	else if (id == "#west")
		return TextureSlot::WEST;
	else if (id == "#up")
		return TextureSlot::UP;
	else if (id == "#down")
		return TextureSlot::DOWN;
	else if (id == "#cross")
		return TextureSlot::CROSS;
	else if (id == "#plant")
		return TextureSlot::PLANT;
	else if (id == "#wall")
		return TextureSlot::WALL;
	else if (id == "#rail")
		return TextureSlot::RAIL;
	else if (id == "#wool")
		return TextureSlot::WOOL;
	else if (id == "#pattern")
		return TextureSlot::PATTERN;
	else if (id == "#pane")
		return TextureSlot::PANE;
	else if (id == "#edge")
		return TextureSlot::EDGE;
	else if (id == "#fan")
		return TextureSlot::FAN;
	else if (id == "#stem")
		return TextureSlot::STEM;
	else if (id == "#upper_stem")
		return TextureSlot::UPPER_STEM;
	else if (id == "#crop")
		return TextureSlot::CROP;
	else if (id == "#dirt")
		return TextureSlot::DIRT;
	else if (id == "#fire")
		return TextureSlot::FIRE;
	else if (id == "#lantern")
		return TextureSlot::LANTERN;
	else if (id == "#platform")
		return TextureSlot::PLATFORM;
	else if (id == "#unsticky")
		return TextureSlot::UNSTICKY;
	else if (id == "#torth")
		return TextureSlot::TORCH;
	else if (id == "#layer0")
		return TextureSlot::LAYER0;
	else if (id == "#lit_log")
		return TextureSlot::LIT_LOG;
	else if (id == "#candle")
		return TextureSlot::CANDLE;
	else if (id == "#inside")
		return TextureSlot::INSIDE;
	else if (id == "#content")
		return TextureSlot::CONTENT;
	else
		return TextureSlot::NONE;
}

//TextureSlot::Slot TextureSlot::getTextureSlot(const std::string& id) noexcept
//{
//	if (id == "#all")
//		return TextureSlot::Slot::ALL;
//	else if (id == "#texture")
//		return TextureSlot::Slot::TEXTURE;
//	else if (id == "#particle")
//		return TextureSlot::Slot::PARTICLE;
//	else if (id == "#end")
//		return TextureSlot::Slot::END;
//	else if (id == "#bottom")
//		return TextureSlot::Slot::BOTTOM;
//	else if (id == "#top")
//		return TextureSlot::Slot::TOP;
//	else if (id == "#front")
//		return TextureSlot::Slot::FRONT;
//	else if (id == "#back")
//		return TextureSlot::Slot::BACK;
//	else if (id == "#side")
//		return TextureSlot::Slot::SIDE;
//	else if (id == "#north")
//		return TextureSlot::Slot::NORTH;
//	else if (id == "#south")
//		return TextureSlot::Slot::SOUTH;
//	else if (id == "#east")
//		return TextureSlot::Slot::EAST;
//	else if (id == "#west")
//		return TextureSlot::Slot::WEST;
//	else if (id == "#up")
//		return TextureSlot::Slot::UP;
//	else if (id == "#down")
//		return TextureSlot::Slot::DOWN;
//	else if (id == "#cross")
//		return TextureSlot::Slot::CROSS;
//	else if (id == "#plant")
//		return TextureSlot::Slot::PLANT;
//	else if (id == "#wall")
//		return TextureSlot::Slot::WALL;
//	else if (id == "#rail")
//		return TextureSlot::Slot::RAIL;
//	else if (id == "#wool")
//		return TextureSlot::Slot::WOOL;
//	else if (id == "#pattern")
//		return TextureSlot::Slot::PATTERN;
//	else if (id == "#pane")
//		return TextureSlot::Slot::PANE;
//	else if (id == "#edge")
//		return TextureSlot::Slot::EDGE;
//	else if (id == "#fan")
//		return TextureSlot::Slot::FAN;
//	else if (id == "#stem")
//		return TextureSlot::Slot::STEM;
//	else if (id == "#upper_stem")
//		return TextureSlot::Slot::UPPER_STEM;
//	else if (id == "#crop")
//		return TextureSlot::Slot::CROP;
//	else if (id == "#dirt")
//		return TextureSlot::Slot::DIRT;
//	else if (id == "#fire")
//		return TextureSlot::Slot::FIRE;
//	else if (id == "#lantern")
//		return TextureSlot::Slot::LANTERN;
//	else if (id == "#platform")
//		return TextureSlot::Slot::PLATFORM;
//	else if (id == "#unsticky")
//		return TextureSlot::Slot::UNSTICKY;
//	else if (id == "#torth")
//		return TextureSlot::Slot::TORCH;
//	else if (id == "#layer0")
//		return TextureSlot::Slot::LAYER0;
//	else if (id == "#lit_log")
//		return TextureSlot::Slot::LIT_LOG;
//	else if (id == "#candle")
//		return TextureSlot::Slot::CANDLE;
//	else if (id == "#inside")
//		return TextureSlot::Slot::INSIDE;
//	else if (id == "#content")
//		return TextureSlot::Slot::CONTENT;
//	else
//		return TextureSlot::Slot::NONE;
//}

TextureSlot::operator std::string()  const noexcept
{
	return '#' + id;
}