#pragma once
#include <streambuf>
namespace std
{
	class membuf : public streambuf
	{
	public:
		membuf(char* buffer, streamsize size);
	protected:
		pos_type seekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which = std::ios_base::in | std::ios_base::out) override;
		pos_type seekpos(pos_type pos, ios_base::openmode = ios_base::in | ios_base::out) override;
	};
}