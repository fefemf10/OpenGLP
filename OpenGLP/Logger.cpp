#include "Logger.hpp"

std::ofstream Logger::openglf;
std::ofstream Logger::debugf;

void Logger::init()
{
	openglf.open("logs\\opengl\\" + currentDateTime() + ".log", std::ios::ate);
	debugf.open("logs\\debug.log", std::ios::out | std::ios::trunc);
}

void Logger::opengl(std::string_view str)
{
	openglf << '[' + currentDateTime() + "] " << str << "\n";
	openglf.flush();
}

void Logger::debug(std::string_view str)
{
	debugf << '[' + currentDateTime() + "] " << str << "\n";
	debugf.flush();
}

void Logger::deinit()
{
	openglf.close();
	debugf.close();
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