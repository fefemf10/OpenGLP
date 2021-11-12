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

TextureSlot::operator std::string()  const noexcept
{
	return '#' + id;
}