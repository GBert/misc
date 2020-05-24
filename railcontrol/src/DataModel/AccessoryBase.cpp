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

#include "DataModel/AccessoryBase.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string AccessoryBase::Serialize() const
	{
		stringstream ss;
		ss << HardwareHandle::Serialize()
			<< ";type=" << static_cast<int>(accessoryType)
			<< ";state=" << static_cast<int>(accessoryState)
			<< ";duration=" << static_cast<int>(duration)
			<< ";inverted=" << static_cast<int>(inverted)
			<< ";lastused=" << lastUsed
			<< ";counter=" << counter;
		return ss.str();
	}

	bool AccessoryBase::Deserialize(const map<string,string>& arguments)
	{
		HardwareHandle::Deserialize(arguments);
		accessoryType = static_cast<AccessoryType>(Utils::Utils::GetIntegerMapEntry(arguments, "type"));
		accessoryState = static_cast<AccessoryState>(Utils::Utils::GetIntegerMapEntry(arguments, "state"));
		duration = static_cast<AccessoryPulseDuration>(Utils::Utils::GetIntegerMapEntry(arguments, "timeout", DefaultAccessoryPulseDuration)); // FIXME: remove in later versions, is only here for conversion
		duration = static_cast<AccessoryPulseDuration>(Utils::Utils::GetIntegerMapEntry(arguments, "duration", DefaultAccessoryPulseDuration));
		inverted = Utils::Utils::GetBoolMapEntry(arguments, "inverted");
		lastUsed = Utils::Utils::GetIntegerMapEntry(arguments, "lastused", 0);
		counter = Utils::Utils::GetIntegerMapEntry(arguments, "counter", 0);
		return true;
	}
} // namespace DataModel
