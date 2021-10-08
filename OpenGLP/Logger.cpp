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
	char buffer[64];                                // ������, � ������� ����� ��������� ������� �����
	time(&rawtime);                               // ������� ���� � ��������
	timeinfo = localtime(&rawtime);               // ������� ��������� �����, �������������� � ���������
	strftime(buffer, sizeof(buffer), "%H.%M.%S %d.%m.%y", timeinfo); // ����������� ������ �������
	return buffer;
}