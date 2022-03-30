#include "Region.hpp"
#include "ZLIB/zlib.h"
namespace nbt
{
    Region::Region() : chunks(1024)
    {
    }
    void Region::serialize(std::fstream& stream)
    {
        stream.seekp(0u);
        stream.write(reinterpret_cast<char*>(locations), sizeof(locations));
        stream.write(reinterpret_cast<char*>(timestamps), sizeof(timestamps));
    }

    void Region::deserialize(std::fstream& stream)
    {
        stream.seekg(0u);
        stream.read(reinterpret_cast<char*>(locations), sizeof(locations));
        stream.read(reinterpret_cast<char*>(timestamps), sizeof(timestamps));
        for (size_t i = 0; i < 1024; ++i)
        {
            stream.seekg(getChunkLocationOffset(locations[i]) * 4096);
            uint32_t lengthChunk{};
            stream.read(reinterpret_cast<char*>(&lengthChunk), sizeof(uint32_t));
            lengthChunk = swapEndian(lengthChunk);
            uint8_t compressType{};
            stream.read(reinterpret_cast<char*>(&compressType), sizeof(uint8_t));
            if (compressType == 2)
            {
                std::vector<unsigned char> compressedData(lengthChunk - 1);
                unsigned long uncompressedSize = 1048576u;
                std::vector<unsigned char> data(uncompressedSize);
                stream.read(reinterpret_cast<char*>(compressedData.data()), lengthChunk - 1);
                uncompress(data.data(), &uncompressedSize, compressedData.data(), lengthChunk - 1);
                std::membuf buffer(reinterpret_cast<char*>(data.data()), uncompressedSize);
                std::iostream stream(&buffer);
                chunks[i].decode(stream);
            }
        }
    }

    void Region::readLT(std::fstream& stream)
    {
        stream.seekg(0u);
        stream.read(reinterpret_cast<char*>(locations), sizeof(locations));
        stream.read(reinterpret_cast<char*>(timestamps), sizeof(timestamps));
    }

    void Region::deserialize(std::fstream& stream, uint8_t x, uint8_t z)
    {
        if (deserialized[z][x])
            return;
        deserialized[z][x] = true;
        stream.seekg(getChunkLocationOffset(locations[getChunkLocation(x, z)]) * 4096);
        uint32_t lengthChunk{};
        stream.read(reinterpret_cast<char*>(&lengthChunk), sizeof(uint32_t));
        lengthChunk = swapEndian(lengthChunk);
        uint8_t compressType{};
        stream.read(reinterpret_cast<char*>(&compressType), sizeof(uint8_t));
        if (compressType == 2)
        {
            std::vector<unsigned char> compressedData(lengthChunk - 1);
            unsigned long uncompressedSize = 1048576u;
            std::vector<unsigned char> data(uncompressedSize);
            stream.read(reinterpret_cast<char*>(compressedData.data()), lengthChunk - 1);
            uncompress(data.data(), &uncompressedSize, compressedData.data(), lengthChunk - 1);
            std::membuf buffer(reinterpret_cast<char*>(data.data()), uncompressedSize);
            std::iostream stream(&buffer);
            chunks[getChunkLocation(x, z)].decode(stream);
        }
    }

    uint16_t Region::getChunkLocation(uint8_t x, uint8_t z) const
    {
        return x + (z << 5);
    }

    uint32_t Region::getChunkLocationOffset(uint32_t location) const
    {
        return swapEndian(location << 8u);
    }

    uint8_t Region::getChunkLocationSize(uint32_t location) const
    {
        return location & 0xFFu;
    }

    uint32_t Region::getChunkTimestamp(int32_t x, int32_t z) const
    {
        return sizeof(locations) + (((x & 31) + ((z & 31) >> 5)) >> 2);
    }
}