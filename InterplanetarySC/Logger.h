#include <string>
#include <iostream>
#include <fstream>

class Logger
{
private:
	std::string filename;
	std::ofstream file;
public:
	Logger(std::string name);
	~Logger(void);
	void logLine(std::string str);
	void open();
	void close();
};

