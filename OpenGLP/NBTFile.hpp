#pragma once
#include "nbt.hpp"
#include "membuf.hpp"
namespace nbt
{
	struct NBTFile
	{
		void serialize(const std::string& path, bool compressed);
		void deserialize(const std::string& path, bool compressed);
		NBT root;
	};
}
