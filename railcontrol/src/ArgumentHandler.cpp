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

#include "ArgumentHandler.h"

ArgumentHandler::ArgumentHandler(const int argc, char* argv[], /*const std::map<std::string,char>& linkMap,*/ const char defaultSwitch)
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

//		std::string argString = arg + 2;
//		if (linkMap.count(argString) == 0)
//		{
//			continue;
//		}
//		const char shortArg = linkMap[argString];
	}
}

bool ArgumentHandler::GetArgumentBool(const char argument)
{
	return (argumentMap.count(argument) == 1);
}

std::string ArgumentHandler::GetArgumentString(const char argument, const std::string& defaultValue)
{
	return (GetArgumentBool(argument) ? argumentMap[argument] : defaultValue);
}
