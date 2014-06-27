#pragma once
#include <string>
#include <iostream>
#include <fstream>

/*
Diese Klasse ermöglicht es einzelne Logdateien zu schreiben um das
Bugfixing zu erleichtern. Zudem kann es auch als Schiffs-Logbuch
eingesetzt werden.
*/
class Logger {
public:
  Logger(std::string name);
  ~Logger(void);
  void logLine(std::string str);
  void open();
  void close();

private:
  std::string filename_;
  std::fstream file_;
};

