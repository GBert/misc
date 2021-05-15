/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2021 Dominik (Teddy) Mahrer - www.railcontrol.org

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

#include <map>
#include <string>

#include "DataModel/Text.h"
#include "Utils/Utils.h"

using std::map;
using std::string;

namespace DataModel
{
	string Text::Serialize() const
	{
		string str;
		str = "objectType=Text;";
		str += LayoutItem::Serialize();
		return str;
	}

	bool Text::Deserialize(const string& serialized)
	{
		map<string, string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Text") != 0)
		{
			return false;
		}
		LayoutItem::Deserialize(arguments);
		SetHeight(Height1);
		SetVisible(VisibleYes);
		return true;
	}
} // namespace DataModel
