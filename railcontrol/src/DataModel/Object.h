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
#include "DataModel/Serializable.h"

namespace DataModel
{
	class Object : protected Serializable
	{
		public:
			Object(const Object&) = delete;
			Object& operator=(const Object&) = delete;

			inline Object()
			:	objectID(ObjectNone),
			 	name(std::to_string(objectID))
			{
			}

			inline Object(const ObjectID objectID)
			:	objectID(objectID),
				name(std::to_string(objectID))
			{
			}

			virtual ~Object()
			{
			}

			virtual ObjectType GetObjectType() const = 0;

			virtual std::string Serialize() const override;

			virtual bool Deserialize(const std::string& serialized) override;

			inline ObjectID GetID() const
			{
				return objectID;
			}

			virtual inline void SetName(const std::string& name)
			{
				this->name = name;
			}

			inline const std::string& GetName() const
			{
				return name;
			}

		protected:
			virtual bool Deserialize(const std::map<std::string,std::string>& arguments);

		private:
			ObjectID objectID;
			std::string name;
	};
} // namespace DataModel

