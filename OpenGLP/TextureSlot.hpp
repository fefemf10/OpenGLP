#pragma once
#include <string>
class TextureSlot
{
public:
    static const TextureSlot NONE;
    static const TextureSlot ALL;
    static const TextureSlot TEXTURE;
    static const TextureSlot PARTICLE;
    static const TextureSlot END;
    static const TextureSlot BOTTOM;
    static const TextureSlot TOP;
    static const TextureSlot FRONT;
    static const TextureSlot BACK;
    static const TextureSlot SIDE;
    static const TextureSlot NORTH;
    static const TextureSlot SOUTH;
    static const TextureSlot EAST;
    static const TextureSlot WEST;
    static const TextureSlot UP;
    static const TextureSlot DOWN;
    static const TextureSlot CROSS;
    static const TextureSlot PLANT;
    static const TextureSlot WALL;
    static const TextureSlot RAIL;
    static const TextureSlot WOOL;
    static const TextureSlot PATTERN;
    static const TextureSlot PANE;
    static const TextureSlot EDGE;
    static const TextureSlot FAN;
    static const TextureSlot STEM;
    static const TextureSlot UPPER_STEM;
    static const TextureSlot CROP;
    static const TextureSlot DIRT;
    static const TextureSlot FIRE;
    static const TextureSlot LANTERN;
    static const TextureSlot PLATFORM;
    static const TextureSlot UNSTICKY;
    static const TextureSlot TORCH;
    static const TextureSlot LAYER0;
    static const TextureSlot LIT_LOG;
    static const TextureSlot CANDLE;
    static const TextureSlot INSIDE;
    static const TextureSlot CONTENT;
    const std::string& id;
    const TextureSlot& parent;
    operator std::string() const noexcept;
};

