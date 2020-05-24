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

#pragma once

#include <map>
#include <string>

#include "DataTypes.h"
#include "Utils/Utils.h"

namespace DataModel
{
	class ObjectIdentifier
	{
		public:
			inline ObjectIdentifier()
			:	objectType(ObjectTypeNone),
				objectID(ObjectNone)
			{}

			inline ObjectIdentifier(const std::string& text)
			{
				*this = text;
			}

			inline ObjectIdentifier(const std::string& track, const std::string& signal)
			{
				ObjectID trackID = Utils::Utils::StringToInteger(track);
				if (trackID > TrackNone)
				{
					objectType = ObjectTypeTrack;
					objectID = trackID;
					return;
				}
				ObjectID signalID = Utils::Utils::StringToInteger(signal);
				if (signalID > SignalNone)
				{
					objectType = ObjectTypeSignal;
					objectID = signalID;
					return;
				}
				objectType = ObjectTypeNone;
				objectID = ObjectNone;
			}

			inline ObjectIdentifier(const ObjectType objectType, const ObjectID objectID)
			:	objectType(objectType),
				objectID(objectID)
			{}

			inline std::string Serialize() const
			{
				return GetObjectTypeAsString() + "=" + std::to_string(objectID);
			}

			inline bool Deserialize(const std::map<std::string,std::string>& arguments)
			{
				objectID = static_cast<TrackID>(Utils::Utils::GetIntegerMapEntry(arguments, "track", ObjectNone));
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
				objectType = ObjectTypeNone;
				return false;
			}

			inline ObjectIdentifier& operator=(const ObjectIdentifier& other) = default;
			inline ObjectIdentifier& operator=(const ObjectType& objectType) { this->objectType = objectType; return *this; }
			inline ObjectIdentifier& operator=(const ObjectID& objectID) { this->objectID = objectID; return *this; }
			inline ObjectIdentifier& operator=(const std::string& text)
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
				objectType = ObjectTypeTrack;
				objectID = Utils::Utils::StringToInteger(text, ObjectNone);;
				return *this;
			}

			inline bool operator==(const ObjectIdentifier& other) const { return this->objectType == other.objectType && this->objectID == other.objectID; }
			inline bool operator!=(const ObjectIdentifier& other) const { return !(*this == other); }

			inline operator std::string() const { return GetObjectTypeAsString() + std::to_string(objectID); }

			inline void Clear() { objectType = ObjectTypeNone; objectID = ObjectNone; }
			inline bool IsSet() const { return objectType != ObjectTypeNone; }
			inline ObjectType GetObjectType() const { return objectType; }
			inline ObjectID GetObjectID() const { return objectID; }

			inline std::string GetObjectTypeAsString() const
			{
				switch (objectType)
				{
					case ObjectTypeTrack:
						return "track";

					case ObjectTypeSignal:
						return "signal";

					default:
						return "object";
				}
			}

			inline std::string GetObjectIdAsString() const
			{
				return std::to_string(objectID);
			}

		private:
			ObjectType objectType;
			ObjectID objectID;
	};
} // namespace DataModel

