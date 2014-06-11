/*
Diese Klasse ermöglicht es einzelne Logdateien zu schreiben um das
Bugfixing zu erleichtern. Zudem kann es auch als Schiffs-Logbuch
eingesetzt werden.
*/
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

