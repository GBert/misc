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
#include <string>

#include "DataModel/Feedback.h"
#include "DataModel/Track.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::string;
using std::to_string;
using std::vector;

namespace DataModel
{
	std::string Track::Serialize() const
	{
		std::string str;
		str = "objectType=Track;";
		str += TrackBase::Serialize();
		str += ";";
		str += LayoutItem::Serialize();
		str += ";";
		str += LockableItem::Serialize();
		str += ";tracktype=";
		str += to_string(trackType);
		return str;
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
		trackType = static_cast<TrackType>(Utils::Utils::GetIntegerMapEntry(arguments, "type", TrackTypeStraight)); // FIXME: remove later 2020-10-27
		trackType = static_cast<TrackType>(Utils::Utils::GetIntegerMapEntry(arguments, "tracktype", trackType));
		switch (trackType)
		{
			case TrackTypeTurn:
			case TrackTypeTunnelEnd:
				SetHeight(Height1);
				break;

			case TrackTypeCrossingLeft:
			case TrackTypeCrossingRight:
			case TrackTypeCrossingSymetric:
				SetHeight(Height2);
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
