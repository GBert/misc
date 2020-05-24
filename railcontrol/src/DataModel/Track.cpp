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

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include "DataModel/Feedback.h"
#include "DataModel/Track.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::vector;

namespace DataModel
{
	std::string Track::Serialize() const
	{
		std::stringstream ss;
		ss << "objectType=Track;"
			<< TrackBase::Serialize()
			<< ";" << LayoutItem::Serialize()
			<< ";" << LockableItem::Serialize()
			<< ";tracktype=" << static_cast<int>(trackType);
		return ss.str();
	}

	bool Track::Deserialize(const std::string& serialized)
	{
		map<string, string> arguments;
		ParseArguments(serialized, arguments);
		string objectType = Utils::Utils::GetStringMapEntry(arguments, "objectType");
		if (objectType.compare("Track") != 0)
		{
			return false;
		}
		LayoutItem::Deserialize(arguments);
		LockableItem::Deserialize(arguments);
		TrackBase::Deserialize(arguments);
		SetWidth(Width1);
		SetVisible(VisibleYes);
		trackType = static_cast<TrackType>(Utils::Utils::GetIntegerMapEntry(arguments, "type", TrackTypeStraight)); // FIXME: remove later
		trackType = static_cast<TrackType>(Utils::Utils::GetIntegerMapEntry(arguments, "tracktype", trackType));
		switch (trackType)
		{
			case TrackTypeTurn:
			case TrackTypeTunnelEnd:
				SetHeight(Height1);
				break;

			default:
				break;
		}
		return true;
	}

	void Track::PublishState() const
	{
		manager->TrackPublishState(this);
	}
} // namespace DataModel
