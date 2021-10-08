#pragma once
#include <fstream>
#include <string>
#pragma warning(disable : 4996)
#include <ctime>
class Logger
{
public:
	static void init();
	static void log(const std::string& log);
	static void deinit();
private:
	static const std::string currentDateTime();
	static std::ofstream file;
};