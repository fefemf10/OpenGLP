#pragma once

enum class State : unsigned char
{
	Empty,
	Loaded,
	Modified,
	Buffered,
	Full
};