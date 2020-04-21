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

#include <iostream>
#include <vector>

#include "ArgumentHandler.h"
#include "Utils/Utils.h"

ArgumentHandler::ArgumentHandler(const int argc, char* argv[], const std::map<std::string,char>& linkMap, const char defaultSwitch)
{
	for (int i = 1; i < argc; ++i)
	{
		char* arg = argv[i];
		if (arg[0] != '-')
		{
			argumentMap[defaultSwitch] = arg;
			continue;
		}

		if (arg[1] != '-')
		{
			argumentMap[arg[1]] = "";
			continue;
		}

		std::string argString = arg + 2;
		std::vector<std::string> parts;
		Utils::Utils::SplitString(argString, "=", parts);
		if (parts.size() < 1)
		{
			continue;
		}
		std::string& argumentLong = parts[0];
		if (linkMap.count(argumentLong) == 0)
		{
			std::cout << "Unknown argument " << argumentLong << std::endl;
			continue;
		}
		char argumentShort = linkMap.at(argumentLong);
		argumentMap[argumentShort] = (parts.size() == 1 ? "" : parts[1]);
	}
}
