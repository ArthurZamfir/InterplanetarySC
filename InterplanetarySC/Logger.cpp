#include "Logger.h"
#include <fstream>


Logger::Logger(std::string name)
{
	filename_ = name;

}
Logger::~Logger(void)
{
	file_.close();
}

void Logger::logLine(std::string str)
{
	if(!file_.is_open())
		file_.open(filename_,std::fstream::app | std::fstream::out);
	file_ << str << '\n';
	file_.close();
}

void Logger::open()
{
	file_.open(filename_);
}

void Logger::close()
{
	file_.close();
}