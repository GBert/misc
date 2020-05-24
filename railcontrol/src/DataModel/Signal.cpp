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

#include "DataModel/Signal.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Signal::Serialize() const
	{
		stringstream ss;
		ss << "objectType=Signal;"
			<< AccessoryBase::Serialize()
			<< ";" << TrackBase::Serialize()
			<< ";" << LayoutItem::Serialize()
			<< ";" << LockableItem::Serialize();
		return ss.str();
	}

	bool Signal::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Signal") != 0)
		{
			return false;
		}

		AccessoryBase::Deserialize(arguments);
		TrackBase::Deserialize(arguments);
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		SetWidth(Width1);
		SetVisible(VisibleYes);
		return true;
	}

	bool Signal::ReleaseInternal(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = LockableItem::Release(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		SetAccessoryState(SignalStateRed);
		PublishState();
		return true;
	}

	void Signal::PublishState() const
	{
		manager->SignalPublishState(ControlTypeInternal, this);
	}
} // namespace DataModel

