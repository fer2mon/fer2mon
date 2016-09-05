#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>

namespace Beastmaster
{
	enum LogEntryType
	{
		Normal,
		Warning,
		Error
	};

	class Log
	{
	private:
		static Log instance;		

		std::ofstream stream;
	public:
		static Log& getInstance();	

		void log(std::string entry, LogEntryType entryType);
	private:
		Log();
		~Log();
		Log(Log const&); //don't implement
		void operator=(Log const&); //don't implement	
	};
}

#endif