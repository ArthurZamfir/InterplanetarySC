#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Logger
{

private:
	string filename;
	ofstream file;

public:

	Logger(string name)
	{
		filename = name;
		file.open(filename);
	}

	~Logger(void)
	{
		file.close();
	}

	void logLine(string str)
	{
		file << str << '\n';
	}

	void open()
	{
		file.open(filename);
	}

	void close()
	{
		file.close();
	}
};

