#pragma once
#include <fstream>
#include <vector>
#include "membuf.hpp"
#include "nbt.hpp"
namespace nbt
{
	template<typename T>
	inline T swapEndian(T value)
	{
		T tmp;
		for (size_t i = 0; i < sizeof(T); ++i)
		{
			char tmpc = reinterpret_cast<char*>(&tmp)[sizeof(T) - i - 1];
			reinterpret_cast<char*>(&tmp)[sizeof(T) - i - 1] = reinterpret_cast<char*>(&value)[i];
			reinterpret_cast<char*>(&value)[i] = tmpc;
		}
		return tmp;
	}
	struct Region
	{
		Region();
		void serialize(std::fstream& stream);
		void deserialize(std::fstream& stream);
		void readLT(std::fstream& stream);
		void deserialize(std::fstream& stream, uint8_t x, uint8_t z);
		uint16_t getChunkLocation(uint8_t x, uint8_t z) const;
		uint32_t getChunkLocationOffset(uint32_t location) const;
		uint8_t getChunkLocationSize(uint32_t location) const;
		uint32_t getChunkTimestamp(int32_t x, int32_t z) const;
		uint32_t locations[1024]{};
		uint32_t timestamps[1024]{};
		std::vector<NBT> chunks;
	};
}