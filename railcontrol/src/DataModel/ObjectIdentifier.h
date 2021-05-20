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
			{
			}

			inline ObjectIdentifier(const std::string& text)
			{
				*this = text;
			}

			inline ObjectIdentifier(const ObjectType objectType, const ObjectID objectID)
			:	objectType(objectType),
				objectID(objectID)
			{
			}

			inline ObjectIdentifier(const std::map<std::string,std::string>& arguments)
			{
				Deserialize(arguments);
			}

			inline std::string Serialize() const
			{
				return GetObjectTypeAsString() + "=" + std::to_string(objectID);
			}

			bool Deserialize(const std::map<std::string,std::string>& arguments);

			inline ObjectIdentifier& operator=(const ObjectIdentifier& other) = default;

			inline void SetObjectType(const ObjectType objectType)
			{
				this->objectType = objectType;
			}

			inline void SetObjectID(const ObjectID objectID)
			{
				this->objectID = objectID;
			}

			ObjectIdentifier& operator=(const std::string& text);

			inline bool operator==(const ObjectIdentifier& other) const
			{
				return this->objectType == other.objectType && this->objectID == other.objectID;
			}

			inline bool operator!=(const ObjectIdentifier& other) const
				{
				return !(*this == other);
			}

			inline operator std::string() const
			{
				return GetObjectTypeAsString() + std::to_string(objectID);
			}

			inline void Clear()
			{
				objectType = ObjectTypeNone;
				objectID = ObjectNone;
			}

			inline bool IsSet() const
			{
				return objectType != ObjectTypeNone;
			}

			inline ObjectType GetObjectType() const
			{
				return objectType;
			}

			inline ObjectID GetObjectID() const
			{
				return objectID;
			}

			std::string GetObjectTypeAsString() const;

			inline std::string GetObjectIdAsString() const
			{
				return std::to_string(objectID);
			}

		private:
			ObjectType objectType;
			ObjectID objectID;
	};
} // namespace DataModel

