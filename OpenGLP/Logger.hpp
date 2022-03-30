#pragma once
#include <fstream>
#include <string_view>
#pragma warning(disable : 4996)
#include <ctime>
class Logger
{
public:
	static void init();
	static void opengl(std::string_view str);
	static void debug(std::string_view str);
	static void deinit();
private:
	static const std::string currentDateTime();
	static std::ofstream openglf;
	static std::ofstream debugf;
};