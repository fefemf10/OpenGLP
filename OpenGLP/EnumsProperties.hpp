#pragma once
#include <GLM/glm.hpp>
#include <string_view>
namespace Enums
{
	enum class PropertiesBlock
	{
		ATTACHMENT,
		AXIS,
		EAST,
		FACE,
		FACING,
		HALF,
		HINGE,
		INSTRUMENT,
		LEAVES,
		NORTH,
		MODE,
		ORIENTATION,
		PART,
		SCULK_SENSOR_PHASE,
		SHAPE,
		SOUTH,
		THICKNESS,
		TILT,
		TYPE,
		VERTICAL_DIRECTION,
		WEST,

		AGE,
		ATTACHED,
		BITES,
		BERRIES,
		BOTTOM,
		CANDLES,
		CHARGES,
		CONDITIONAL,
		DELAY,
		DISARMED,
		DISTANCE,
		DOWN,
		DRAG,
		EGGS,
		ENABLED,
		EXTENDED,
		EYE,
		HANGING,
		HAS_BOOK,
		HAS_BOTTLE_0,
		HAS_BOTTLE_1,
		HAS_BOTTLE_2,
		HAS_RECORD,
		HATCH,
		HONEY_LEVEL,
		IN_WALL,
		INVERTED,
		LAYERS,
		LEVEL,
		LIT,
		LOCKED,
		MOISTURE,
		NOTE,
		OCCUPIED,
		OPEN,
		PERSISTENT,
		PICKLES,
		POWER,
		POWERED,
		ROTATION,
		SHORT,
		SIGNAL_FIRE,
		SNOWY,
		STAGE,
		TRIGGERED,
		UNSTABLE,
		UP,
		WATERLOGGED,
	};
	constexpr const char* sPropertiesBlock[] =
	{
		"attachment",
		"axis",
		"east",
		"face",
		"facing",
		"half",
		"hinge",
		"instrument",
		"leaves",
		"north",
		"mode",
		"orientation",
		"part",
		"sculk_sensor_phase",
		"shape",
		"south",
		"thickness",
		"tilt",
		"type",
		"vertical_direction",
		"west",
		"age",
		"attached",
		"bites",
		"berries",
		"bottom",
		"candles",
		"charges",
		"conditional",
		"delay",
		"disarmed",
		"distance",
		"down",
		"drag",
		"eggs",
		"enabled",
		"extended",
		"eye",
		"hanging",
		"has_book",
		"has_bottle_0",
		"has_bottle_1",
		"has_bottle_2",
		"has_record",
		"hatch",
		"honey_level",
		"in_wall",
		"inverted",
		"layers",
		"level",
		"lit",
		"locked",
		"moisture",
		"note",
		"occupied",
		"open",
		"persistent",
		"pickles",
		"power",
		"powered",
		"rotation",
		"short",
		"signal_fire",
		"snowy",
		"stage",
		"triggered",
		"unstable",
		"up",
		"waterlogged"
	};
	constexpr PropertiesBlock iPropertiesBlock(std::string_view s)
	{
		for (uint32_t i = 0; i < 69; ++i)
		{
			if (sPropertiesBlock[i] == s)
				return static_cast<PropertiesBlock>(i);
		}
		return static_cast<PropertiesBlock>(0);
	}
	enum class Attachment : unsigned char
	{
		CEILING,
		DOUBLE_WALL,
		FLOOR,
		SINGLE_WALL
	};
	constexpr const char* sAttachment[] =
	{
		"ceiling",
		"double_wall",
		"floor",
		"single_wall"
	};
	constexpr Attachment iAttachment(std::string_view s)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			if (sAttachment[i] == s)
				return static_cast<Attachment>(i);
		}
		return static_cast<Attachment>(0);
	}
	enum class Axis : unsigned char
	{
		X,
		Y,
		Z
	};
	constexpr const char* sAxis[] =
	{
		"x",
		"y",
		"z"
	};
	constexpr Axis iAxis(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sAxis[i] == s)
				return static_cast<Axis>(i);
		}
		return static_cast<Axis>(0);
	}
	enum class East : unsigned char
	{
		NONE,
		SIDE,
		UP
	};
	constexpr const char* sEast[] =
	{
		"none",
		"side",
		"up"
	};
	constexpr East iEast(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sEast[i] == s)
				return static_cast<East>(i);
		}
		return static_cast<East>(0);
	}
	enum class Face : unsigned char
	{
		CEILING,
		FLOOR,
		WALL
	};
	constexpr const char* sFace[] =
	{
		"ceiling",
		"floor",
		"wall"
	};
	constexpr Face iFace(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sFace[i] == s)
				return static_cast<Face>(i);
		}
		return static_cast<Face>(0);
	}
	enum class Direction : unsigned char
	{
		EAST = 0x01,
		WEST = 0x02,
		UP = 0x04,
		DOWN = 0x08,
		SOUTH = 0x10,
		NORTH = 0x20
	};
	/*enum class Culling : unsigned short
	{
		EW,
		EU,
		ED,
		ES,
		EN,
		WU,
		WD,
		WS,
		WN,
		UD,
		US,
		UN,
		DS,
		DN,
		SN,
	};*/
	enum class Culling : short
	{
		EAST,
		WEST,
		UP,
		DOWN,
		SOUTH,
		NORTH
	};
	constexpr Direction getFaceCull(Culling c)
	{
		switch (c)
		{
		case Enums::Culling::EAST:
			return Enums::Direction::EAST;
		case Enums::Culling::WEST:
			return Enums::Direction::WEST;
		case Enums::Culling::UP:
			return Enums::Direction::UP;
		case Enums::Culling::DOWN:
			return Enums::Direction::DOWN;
		case Enums::Culling::SOUTH:
			return Enums::Direction::SOUTH;
		case Enums::Culling::NORTH:
			return Enums::Direction::NORTH;
		}
	}
	constexpr Culling getCullFace(Direction d)
	{
		switch (d)
		{
		case Enums::Direction::EAST:
			return Enums::Culling::EAST;
		case Enums::Direction::WEST:
			return Enums::Culling::WEST;
		case Enums::Direction::UP:
			return Enums::Culling::UP;
		case Enums::Direction::DOWN:
			return Enums::Culling::DOWN;
		case Enums::Direction::SOUTH:
			return Enums::Culling::SOUTH;
		case Enums::Direction::NORTH:
			return Enums::Culling::NORTH;
		}
	}
	constexpr const char* sFacing[] =
	{
		"east",
		"west",
		"up",
		"down",
		"south",
		"north"
	};
	constexpr Direction iFacing(std::string_view s)
	{
		for (uint32_t i = 0; i < 6; ++i)
		{
			if (sFacing[i] == s)
				return static_cast<Direction>(i);
		}
		return static_cast<Direction>(0);
	}
	constexpr glm::ivec3 getVecFacing(Direction d)
	{
		switch (d)
		{
		case Enums::Direction::EAST:
			return glm::ivec3(1, 0, 0);
		case Enums::Direction::WEST:
			return glm::ivec3(-1, 0, 0);
		case Enums::Direction::UP:
			return glm::ivec3(0, 1, 0);
		case Enums::Direction::DOWN:
			return glm::ivec3(0, -1, 0);
		case Enums::Direction::SOUTH:
			return glm::ivec3(0, 0, 1);
		case Enums::Direction::NORTH:
			return glm::ivec3(0, 0, -1);
		}
		return glm::ivec3();
	}
	constexpr Direction getFacingVec(glm::ivec3 d)
	{
		if (glm::ivec3(1, 0, 0) == d)
			return Enums::Direction::EAST;
		else if (glm::ivec3(-1, 0, 0) == d)
			return Enums::Direction::WEST;
		else if (glm::ivec3(0, 1, 0) == d)
			return Enums::Direction::UP;
		else if (glm::ivec3(0, -1, 0) == d)
			return Enums::Direction::DOWN;
		else if (glm::ivec3(0, 0, 1) == d)
			return Enums::Direction::SOUTH;
		else if (glm::ivec3(0, 0, -1) == d)
			return Enums::Direction::NORTH;
		return static_cast<Direction>(0);
	}
	enum class Half : unsigned char
	{
		LOWER,
		UPPER,
		BOTTOM,
		TOP
	};
	constexpr const char* sHalf[] =
	{
		"lower",
		"upper",
		"bottom",
		"top"
	};
	constexpr Half iHalf(std::string_view s)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			if (sHalf[i] == s)
				return static_cast<Half>(i);
		}
		return static_cast<Half>(0);
	}
	enum class Hinge : unsigned char
	{
		LEFT,
		RIGHT
	};
	constexpr const char* sHinge[] =
	{
		"left",
		"right"
	};
	constexpr Hinge iHinge(std::string_view s)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			if (sHinge[i] == s)
				return static_cast<Hinge>(i);
		}
		return static_cast<Hinge>(0);
	}
	enum class Instrument : unsigned char
	{
		BANJO,
		BASEDRUM,
		BASS,
		BELL,
		BIT,
		CHIME,
		COW_BELL,
		DIGERIDOO,
		FLUTE,
		GUITAR,
		HARP,
		HAT,
		IRON_XYLOPHONE,
		SNARE,
		XYLOPHONE
	};
	constexpr const char* sInstrument[] =
	{
		"banjo",
		"basedrum",
		"bass",
		"bell",
		"bit",
		"chime",
		"cow_bell",
		"digeridoo",
		"flute",
		"guitar",
		"harp",
		"hat",
		"iron_xylophone",
		"snare",
		"xylophone"
	};
	constexpr Instrument iInstrument(std::string_view s)
	{
		for (uint32_t i = 0; i < 15; ++i)
		{
			if (sInstrument[i] == s)
				return static_cast<Instrument>(i);
		}
		return static_cast<Instrument>(0);
	}
	enum class Leaves : unsigned char
	{
		LARGE,
		NONE,
		SMALL
	};
	constexpr const char* sLeaves[] =
	{
		"large",
		"none",
		"small"
	};
	constexpr Leaves iLeaves(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sLeaves[i] == s)
				return static_cast<Leaves>(i);
		}
		return static_cast<Leaves>(0);
	}
	enum class North : unsigned char
	{
		NONE,
		SIDE,
		UP
	};
	constexpr const char* sNorth[] =
	{
		"none",
		"side",
		"up"
	};
	constexpr North iNorth(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sNorth[i] == s)
				return static_cast<North>(i);
		}
		return static_cast<North>(0);
	}
	enum class Mode : unsigned char
	{
		COMPARE,
		SUBTRACT,
		CORNER,
		DATA,
		LOAD,
		SAVE
	};
	constexpr const char* sMode[] =
	{
		"compare",
		"subtract",
		"corner",
		"data",
		"load",
		"save"
	};
	constexpr Mode iMode(std::string_view s)
	{
		for (uint32_t i = 0; i < 6; ++i)
		{
			if (sMode[i] == s)
				return static_cast<Mode>(i);
		}
		return static_cast<Mode>(0);
	}
	enum class Orientation : unsigned char
	{
		DOWN_EAST,
		DOWN_NORTH,
		DOWN_SOUTH,
		DOWN_WEST,
		EAST_UP,
		NORTH_UP,
		SOUTH_UP,
		UP_EAST,
		UP_NORTH,
		UP_SOUTH,
		UP_WEST,
		WEST_UP
	};
	constexpr const char* sOrientation[] =
	{
		"down_east",
		"down_north",
		"down_south",
		"down_west",
		"east_up",
		"north_up",
		"south_up",
		"up_east",
		"up_north",
		"up_south",
		"up_west",
		"west_up"
	};
	constexpr Orientation iOrientation(std::string_view s)
	{
		for (uint32_t i = 0; i < 12; ++i)
		{
			if (sOrientation[i] == s)
				return static_cast<Orientation>(i);
		}
		return static_cast<Orientation>(0);
	}
	enum class Part : unsigned char
	{
		FOOT,
		HEAD
	};
	constexpr const char* sPart[] =
	{
		"foot",
		"head"
	};
	constexpr Part iPart(std::string_view s)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			if (sPart[i] == s)
				return static_cast<Part>(i);
		}
		return static_cast<Part>(0);
	}
	enum class SculkSensorPhase : unsigned char
	{
		INACTIVE,
		ACTIVE,
		COOLDOWN
	};
	constexpr const char* sSculkSensorPhase[] =
	{
		"inactive",
		"active",
		"cooldown"
	};
	constexpr SculkSensorPhase iSculkSensorPhase(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sSculkSensorPhase[i] == s)
				return static_cast<SculkSensorPhase>(i);
		}
		return static_cast<SculkSensorPhase>(0);
	}
	enum class Shape : unsigned char
	{
		ASCENDING_EAST,
		ASCENDING_NORTH,
		ASCENDING_SOUTH,
		ASCENDING_WEST,
		EAST_WEST,
		NORTH_SOUTH,
		NORTH_EAST,
		NORTH_WEST,
		SOUTH_EAST,
		SOUTH_WEST,
		INNER_LEFT,
		INNER_RIGHT,
		OUTER_LEFT,
		OUTER_RIGHT,
		STRAIGHT,
	};
	constexpr const char* sShape[] =
	{
		"ascending_east",
		"ascending_north",
		"ascending_south",
		"ascending_west",
		"east_west",
		"north_south",
		"north_east",
		"north_west",
		"south_east",
		"south_west",
		"inner_left",
		"inner_right",
		"outer_left",
		"outer_right",
		"straight"
	};
	constexpr Shape iShape(std::string_view s)
	{
		for (uint32_t i = 0; i < 15; ++i)
		{
			if (sShape[i] == s)
				return static_cast<Shape>(i);
		}
		return static_cast<Shape>(0);
	}
	enum class South : unsigned char
	{
		NONE,
		SIDE,
		UP
	};
	constexpr const char* sSouth[] =
	{
		"none",
		"side",
		"up"
	};
	constexpr South iSouth(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sSouth[i] == s)
				return static_cast<South>(i);
		}
		return static_cast<South>(0);
	}
	enum class Thickness : unsigned char
	{
		TIP_MERGE,
		TIP,
		FRUSTUM,
		MIDDLE,
		BASE
	};
	constexpr const char* sThickness[] =
	{
		"tip_merge",
		"tip",
		"frustum",
		"middle",
		"base"
	};
	constexpr Thickness iThickness(std::string_view s)
	{
		for (uint32_t i = 0; i < 5; ++i)
		{
			if (sThickness[i] == s)
				return static_cast<Thickness>(i);
		}
		return static_cast<Thickness>(0);
	}
	enum class Tilt : unsigned char
	{
		NONE,
		UNSTABLE,
		PARTIAL,
		FULL
	};
	constexpr const char* sTilt[] =
	{
		"none",
		"unstable",
		"partial",
		"full"
	};
	constexpr Tilt iTilt(std::string_view s)
	{
		for (uint32_t i = 0; i < 4; ++i)
		{
			if (sTilt[i] == s)
				return static_cast<Tilt>(i);
		}
		return static_cast<Tilt>(0);
	}
	enum class Type : unsigned char
	{
		NORMAL,
		STICKY,
		LEFT,
		RIGHT,
		SINGLE,
		BOTTOM,
		DOUBLE,
		TOP
	};
	constexpr const char* sType[] =
	{
		"normal",
		"sticky",
		"left",
		"right",
		"single",
		"bottom",
		"doubled",
		"top"
	};
	constexpr Type iType(std::string_view s)
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			if (sType[i] == s)
				return static_cast<Type>(i);
		}
		return static_cast<Type>(0);
	}
	enum class VerticalDirection : unsigned char
	{
		UP,
		DOWN
	};
	constexpr const char* sVerticalDirection[] =
	{
		"up",
		"down"
	};
	constexpr VerticalDirection iVerticalDirection(std::string_view s)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			if (sVerticalDirection[i] == s)
				return static_cast<VerticalDirection>(i);
		}
		return static_cast<VerticalDirection>(0);
	}
	enum class West : unsigned char
	{
		NONE,
		SIDE,
		UP
	};
	constexpr const char* sWest[] =
	{
		"none",
		"side",
		"up"
	};
	constexpr West iWest(std::string_view s)
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			if (sWest[i] == s)
				return static_cast<West>(i);
		}
		return static_cast<West>(0);
	}
}