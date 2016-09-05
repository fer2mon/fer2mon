#include "Log.hpp"

#ifdef WIN32
#include <windows.h>
#endif

using namespace Beastmaster;

Log Log::instance;

Log& Log::getInstance()
{
	return Log::instance;
}

Log::Log()
{
	this->stream = std::ofstream("log.txt", std::ofstream::out);
}
Log::~Log()
{
	this->stream.close();
}
void Log::log(std::string entry, LogEntryType entryType)
{

#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD flags;
	switch (entryType)
	{
	case LogEntryType::Normal: flags = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
	case LogEntryType::Warning: flags = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
	case LogEntryType::Error: flags = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
	default: FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
	}
	SetConsoleTextAttribute(hConsole, flags);
#endif

	std::string prefixedEntry;
	switch (entryType)
	{
	case LogEntryType::Normal: prefixedEntry = entry; break;
	case LogEntryType::Warning: prefixedEntry = "Warning: " + entry; break;
	case LogEntryType::Error: prefixedEntry = "Error: " + entry; break;
	default: prefixedEntry = entry; break;

	}

	std::cout << prefixedEntry.c_str() << std::endl;
	this->stream << prefixedEntry.c_str() << std::endl;
}