#include "Logger.h"
#include <fstream>


Logger::Logger(std::string name)
{
	filename = name;

}
Logger::~Logger(void)
{
	file.close();
}

void Logger::logLine(std::string str)
{
	if(!file.is_open())
		file.open(filename,std::fstream::app | std::fstream::out);
	file << str << '\n';
	file.close();
}

void Logger::open()
{
	file.open(filename);
}

void Logger::close()
{
	file.close();
}