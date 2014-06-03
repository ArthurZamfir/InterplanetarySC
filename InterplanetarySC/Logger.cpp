#include "Logger.h"


Logger::Logger(std::string name)
{
	filename = name;
	file.open(filename);
}
Logger::~Logger(void)
{
	file.close();
}

void Logger::logLine(std::string str)
{
	file << str << '\n';
}

void Logger::open()
{
	file.open(filename);
}

void Logger::close()
{
	file.close();
}