/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#pragma once

#include <map>
#include <string>

#include "Utils/Utils.h"

class ArgumentHandler
{
	public:
		ArgumentHandler() = delete;
		ArgumentHandler(const int argc, char* argv[], const std::map<std::string,char>& linkMap, const char defaultSwitch);

		bool GetArgumentBool(const char argument)
		{
			return (argumentMap.count(argument) == 1);
		}

		std::string GetArgumentString(const char argument, const std::string& defaultValue = "")
		{
			if (GetArgumentBool(argument) == false)
			{
				return defaultValue;
			}
			std::string& value = argumentMap[argument];
			if (value.size() == 0)
			{
				return defaultValue;
			}

			return value;
		}

		int GetArgumentInt(const char argument, const int defaultValue = 0)
		{
			return Utils::Utils::StringToInteger(GetArgumentString(argument, std::to_string(defaultValue)));
		}

	private:
		std::map<char,std::string> argumentMap;
};
