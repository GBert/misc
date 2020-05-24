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

#include "DataModel/Accessory.h"
#include "DataModel/LockableItem.h"
#include "DataModel/Serializable.h"
#include "DataTypes.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Relation : protected Serializable, public LockableItem
	{
		public:
			enum Type : unsigned char
			{
				TypeCalculate = 0,
				TypeStreetAtLock = ObjectTypeStreet << 3,
				TypeStreetAtUnlock = (ObjectTypeStreet << 3) + 1,
				TypeLocoSlave = ObjectTypeLoco << 3,
				TypeFeedbackAtSet = ObjectTypeFeedback << 3,
				TypeFeedbackAtUnset = (ObjectTypeFeedback << 3) + 1,
			};

			typedef unsigned char Data;
			static const Data DefaultData = 0;

			Relation(Manager* manager,
				const ObjectType objectType1,
				const ObjectID objectID1,
				const ObjectType objectType2,
				const ObjectID objectID2,
				const Type type,
				const Priority priority,
				const unsigned char data)
			:	manager(manager),
				objectType1(objectType1),
			 	objectID1(objectID1),
				objectType2(objectType2),
				objectID2(objectID2),
				type(type),
				priority(priority),
				data(data)
			{}

			Relation(Manager* manager,
				const std::string& serialized)
			:	manager(manager),
				data(0)
			{
				Deserialize(serialized);
			}

			virtual ~Relation() {}

			virtual std::string Serialize() const override;
			virtual bool Deserialize(const std::string& serialized) override;

			ObjectID ObjectID1() const { return objectID1; }
			void ObjectID1(ObjectID objectID1) { this->objectID1 = objectID1; }
			ObjectType ObjectType2() const { return objectType2; }
			ObjectID ObjectID2() const { return objectID2; }
			Type GetType() const { return type; }
			Priority GetPriority() const { return priority; }
			Data GetData() const { return data; }

			bool Reserve(Logger::Logger* logger, const LocoID locoID) override;
			bool Lock(Logger::Logger* logger, const LocoID locoID) override;
			bool Release(Logger::Logger* logger, const LocoID locoID) override;
			bool Execute(Logger::Logger* logger, const LocoID locoID, const Delay delay);

		private:
			LockableItem* GetObject2();

			Manager* manager;
			ObjectType objectType1;
			ObjectID objectID1;
			ObjectType objectType2;
			ObjectID objectID2;
			Type type;
			Priority priority;
			Data data;
	};
} // namespace DataModel

