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
#include <sstream>

#include "DataModel/Accessory.h"
#include "Utils/Utils.h"

using std::map;
using std::string;

namespace DataModel
{
	std::string Accessory::Serialize() const
	{
		string str = "objectType=Accessory;";
		str += AccessoryBase::Serialize();
		str += ";" + LayoutItem::Serialize();
		str += ";" + LockableItem::Serialize();
		return str;
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
		AccessoryBase::Deserialize(arguments);
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		SetWidth(Width1);
		SetHeight(Height1);
		SetVisible(VisibleYes);
		return true;
	}

	Accessory& Accessory::operator=(const Hardware::AccessoryCacheEntry& accessory)
	{
		SetControlID(accessory.GetControlID());
		SetName(accessory.GetName());
		SetAddress(accessory.GetAddress());
		SetProtocol(accessory.GetProtocol());
		SetMatchKey(accessory.GetMatchKey());
		return *this;
	}
} // namespace DataModel
