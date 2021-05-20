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

#include "DataModel/ObjectIdentifier.h"
#include "Utils/Utils.h"

namespace DataModel
{
	bool ObjectIdentifier::Deserialize(const std::map<std::string, std::string>& arguments)
	{
		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "track", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeTrack;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "signal", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeSignal;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "switch", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeSwitch;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "accessory", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeAccessory;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "feedback", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeFeedback;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "route", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeRoute;
			return true;
		}

		objectID = static_cast<ObjectID>(Utils::Utils::GetIntegerMapEntry(arguments, "text", ObjectNone));
		if (objectID != ObjectNone)
		{
			objectType = ObjectTypeText;
			return true;
		}

		objectType = ObjectTypeNone;
		return false;
	}

	ObjectIdentifier& ObjectIdentifier::operator=(const std::string& text)
	{
		if (text.substr(0, 5).compare("track") == 0)
		{
			objectType = ObjectTypeTrack;
			objectID = Utils::Utils::StringToInteger(text.substr(5), ObjectNone);
			return *this;
		}
		if (text.substr(0, 6).compare("signal") == 0)
		{
			objectType = ObjectTypeSignal;
			objectID = Utils::Utils::StringToInteger(text.substr(6), ObjectNone);
			return *this;
		}
		if (text.substr(0, 5).compare("route") == 0)
		{
			objectType = ObjectTypeRoute;
			objectID = Utils::Utils::StringToInteger(text.substr(5), ObjectNone);
			return *this;
		}
		if (text.substr(0, 4).compare("loco") == 0)
		{
			objectType = ObjectTypeLoco;
			objectID = Utils::Utils::StringToInteger(text.substr(4), ObjectNone);
			return *this;
		}
		if (text.substr(0, 8).compare("feedback") == 0)
		{
			objectType = ObjectTypeFeedback;
			objectID = Utils::Utils::StringToInteger(text.substr(8), ObjectNone);
			return *this;
		}
		if (text.substr(0, 9).compare("accessory") == 0)
		{
			objectType = ObjectTypeAccessory;
			objectID = Utils::Utils::StringToInteger(text.substr(9), ObjectNone);
			return *this;
		}
		if (text.substr(0, 6).compare("switch") == 0)
		{
			objectType = ObjectTypeSwitch;
			objectID = Utils::Utils::StringToInteger(text.substr(6), ObjectNone);
			return *this;
		}
		if (text.substr(0, 5).compare("layer") == 0)
		{
			objectType = ObjectTypeLayer;
			objectID = Utils::Utils::StringToInteger(text.substr(5), ObjectNone);
			return *this;
		}
		if (text.substr(0, 7).compare("cluster") == 0)
		{
			objectType = ObjectTypeCluster;
			objectID = Utils::Utils::StringToInteger(text.substr(7), ObjectNone);
			return *this;
		}
		if (text.substr(0, 9).compare("timetable") == 0)
		{
			objectType = ObjectTypeTimeTable;
			objectID = Utils::Utils::StringToInteger(text.substr(9), ObjectNone);
			return *this;
		}
		if (text.substr(0, 4).compare("text") == 0)
		{
			objectType = ObjectTypeText;
			objectID = Utils::Utils::StringToInteger(text.substr(4), ObjectNone);
			return *this;
		}
		objectType = ObjectTypeTrack;
		objectID = Utils::Utils::StringToInteger(text, ObjectNone);
		return *this;
	}

	std::string ObjectIdentifier::GetObjectTypeAsString() const
	{
		switch (objectType)
		{
			case ObjectTypeNone:
				return "none";

			case ObjectTypeLoco:
				return "loco";

			case ObjectTypeTrack:
				return "track";

			case ObjectTypeFeedback:
				return "feedback";

			case ObjectTypeAccessory:
				return "accessory";

			case ObjectTypeSwitch:
				return "switch";

			case ObjectTypeRoute:
				return "route";

			case ObjectTypeLayer:
				return "layer";

			case ObjectTypeSignal:
				return "signal";

			case ObjectTypeCluster:
				return "cluster";

			case ObjectTypeTimeTable:
				return "timetable";

			case ObjectTypeText:
				return "text";
		}
		return "object";
	}
} // namespace DataModel

