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

#include <map>
#include <sstream>

#include "DataModel/Accessory.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Accessory::Serialize() const
	{
		stringstream ss;
		ss << "objectType=Accessory;" << SerializeWithoutType();
		return ss.str();
	}

	std::string Accessory::SerializeWithoutType() const
	{
		stringstream ss;
		ss << LayoutItem::Serialize()
			<< ";" << LockableItem::Serialize()
			<< ";" << HardwareHandle::Serialize()
			<< ";type=" << static_cast<int>(type)
			<< ";state=" << static_cast<int>(state)
			<< ";duration=" << static_cast<int>(duration)
			<< ";inverted=" << static_cast<int>(inverted)
			<< ";lastused=" << lastUsed
			<< ";counter=" << counter;
		return ss.str();
	}

	bool Accessory::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Accessory") != 0)
		{
			return false;
		}

		return Deserialize(arguments);
	}

	bool Accessory::Deserialize(const map<string,string>& arguments)
	{
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		HardwareHandle::Deserialize(arguments);
		SetWidth(Width1);
		SetHeight(Height1);
		SetVisible(VisibleYes);
		type = Utils::Utils::GetIntegerMapEntry(arguments, "type");
		state = Utils::Utils::GetIntegerMapEntry(arguments, "state");
		duration = Utils::Utils::GetIntegerMapEntry(arguments, "timeout", DefaultAccessoryDuration); // FIXME: remove in later versions, is only here for conversion
		duration = Utils::Utils::GetIntegerMapEntry(arguments, "duration", DefaultAccessoryDuration);
		inverted = Utils::Utils::GetBoolMapEntry(arguments, "inverted");
		lastUsed = Utils::Utils::GetIntegerMapEntry(arguments, "lastused", 0);
		counter = Utils::Utils::GetIntegerMapEntry(arguments, "counter", 0);
		return true;
	}
} // namespace DataModel
