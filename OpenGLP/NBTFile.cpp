#include "NBTFile.hpp"
#include "zlc/gzipcomplete.hpp"
#include <fstream>
namespace nbt
{
	void NBTFile::serialize(const std::string& path, bool compressed)
	{
		std::ofstream file(path, std::ios::out | std::ios::binary);
		if (compressed)
		{
			unsigned long uncompressedSize = 1048576u;
			std::string data;
			data.resize(uncompressedSize);
			std::membuf buffer(reinterpret_cast<char*>(data.data()), uncompressedSize);
			std::iostream streams(&buffer);
			root.encode(streams);

			zlibcomplete::GZipCompressor c;
			std::string compressedData = c.compress(data);
			file.write(reinterpret_cast<char*>(compressedData.data()), compressedData.size());
		}
		else
		{
			root.encode(file);
		}
		file.close();
	}

	void NBTFile::deserialize(const std::string& path, bool compressed)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (compressed)
		{
			file.seekg(0, std::ios::end);
			std::streamsize sizeFile{ file.tellg() };
			file.seekg(0, std::ios::beg);
			std::string compressedData;
			compressedData.resize(sizeFile);
			file.read(reinterpret_cast<char*>(compressedData.data()), static_cast<unsigned long>(sizeFile));
			zlibcomplete::GZipDecompressor d;
			std::string data = d.decompress(compressedData);
			std::membuf buffer(reinterpret_cast<char*>(data.data()), data.size());
			std::iostream streams(&buffer);
			root.decode(streams);
		}
		else
		{
			root.decode(file);
		}
		file.close();
	}
}