#include "membuf.hpp"

namespace std
{
	membuf::membuf(char* buffer, std::streamsize size)
	{
		setg(buffer, buffer, buffer + size);
		setp(buffer, buffer, buffer + size);
	}
	streambuf::pos_type membuf::seekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which)
	{
		if (dir == std::ios_base::cur)
			gbump(off);
		else if (dir == std::ios_base::end)
			setg(eback(), egptr() + off, egptr());
		else if (dir == std::ios_base::beg)
			setg(eback(), eback() + off, egptr());
		return gptr() - eback();
	}
	streambuf::pos_type membuf::seekpos(pos_type pos, ios_base::openmode)
	{
		setg(eback(), eback() + pos, egptr());
		return gptr() - eback();
	}
}