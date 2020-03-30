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

#include <vector>

#include "DataModel/Serializable.h"
#include "Utils/Utils.h"

using std::string;
using std::map;
using std::vector;

namespace DataModel
{
	void Serializable::ParseArguments(const string& serialized, map<string, string>& arguments)
	{
		vector<string> parts;
		Utils::Utils::SplitString(serialized, ";", parts);
		for (auto part : parts)
		{
			if (part.length() == 0)
			{
				continue;
			}
			vector<string> keyValue;
			Utils::Utils::SplitString(part, "=", keyValue);
			if (keyValue.size() < 2)
			{
				continue;
			}
			string value = keyValue[1];
			arguments[keyValue[0]] = value;
		}
	}
}
