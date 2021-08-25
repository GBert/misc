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

#include "DataModel/HardwareHandle.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::to_string;

namespace DataModel
{
	std::string HardwareHandle::Serialize() const
	{
		string str = "controlID=" + to_string(controlID);
		str += ";protocol=" + to_string(protocol);
		str += ";address=" + to_string(address);
		return str;
	}

	bool HardwareHandle::Deserialize(const map<string,string>& arguments)
	{
		controlID = Utils::Utils::GetIntegerMapEntry(arguments, "controlID", ControlIdNone);
		protocol = static_cast<Protocol>(Utils::Utils::GetIntegerMapEntry(arguments, "protocol", ProtocolNone));
		address = Utils::Utils::GetIntegerMapEntry(arguments, "address");
		return true;
	}
} // namespace DataModel
