#include "Logger.hpp"

std::ofstream Logger::file;

void Logger::init()
{
	file.open("logs\\" + currentDateTime() + ".txt", std::ios::ate);
}

void Logger::log(const std::string& log)
{
	file << '[' + currentDateTime() + "] " << log << "\n";
	file.flush();
}

void Logger::deinit()
{
	file.close();
}

const std::string Logger::currentDateTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[64];                                // строка, в которой будет храниться текущее время
	time(&rawtime);                               // текущая дата в секундах
	timeinfo = localtime(&rawtime);               // текущее локальное время, представленное в структуре
	strftime(buffer, sizeof(buffer), "%H.%M.%S %d.%m.%y", timeinfo); // форматируем строку времени
	return buffer;
}