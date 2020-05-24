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

#include "DataModel/HardwareHandle.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string HardwareHandle::Serialize() const
	{
		stringstream ss;
		ss << "controlID=" << static_cast<int>(controlID)
			<< ";protocol=" << static_cast<int>(protocol)
			<< ";address=" << static_cast<int>(address);
		return ss.str();
	}

	bool HardwareHandle::Deserialize(const map<string,string>& arguments)
	{
		controlID = Utils::Utils::GetIntegerMapEntry(arguments, "controlID", ControlIdNone);
		protocol = static_cast<Protocol>(Utils::Utils::GetIntegerMapEntry(arguments, "protocol", ProtocolNone));
		address = Utils::Utils::GetIntegerMapEntry(arguments, "address");
		return true;
	}
} // namespace DataModel
