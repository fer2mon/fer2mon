#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <sstream>
#include <vector>

namespace Beastmaster
{
	class Utility
	{
	public:
		static std::vector<std::string> split(const std::string &s, char delim) 
		{
			std::stringstream ss(s);
			std::string item;
			std::vector<std::string> tokens;
			while (std::getline(ss, item, delim)) 
			{
				tokens.push_back(item);
			}
			return tokens;
		}
	};
}

#endif