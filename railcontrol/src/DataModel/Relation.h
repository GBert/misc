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

#include "DataModel/Accessory.h"
#include "DataModel/LockableItem.h"
#include "DataModel/ObjectIdentifier.h"
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
				TypeLocoSlave = ObjectTypeLoco << 3,
				TypeTrackSignal = ObjectTypeTrack << 3,
				TypeFeedbackAtSet = ObjectTypeFeedback << 3,
				TypeFeedbackAtUnset = (ObjectTypeFeedback << 3) + 1,
				TypeRouteAtLock = ObjectTypeRoute << 3,
				TypeRouteAtUnlock = (ObjectTypeRoute << 3) + 1,
				TypeClusterTrack = (ObjectTypeCluster << 3),
				TypeClusterSignal = (ObjectTypeCluster << 3) + 1, // FIXME: remove again later 2021-02-10
			};

			typedef unsigned short Data;
			static const Data DefaultData = 0;

			inline Relation(Manager* manager,
				const ObjectIdentifier& object1,
				const ObjectIdentifier& object2,
				const Type type,
				const Priority priority = 0,
				const unsigned short data = 0)
			:	manager(manager),
			 	object1(object1),
			 	object2(object2),
				type(type),
				priority(priority),
				data(data)
			{
			}

			inline Relation(Manager* manager,
				const std::string& serialized)
			:	manager(manager),
				data(0)
			{
				Deserialize(serialized);
			}

			virtual ~Relation() {}

			virtual std::string Serialize() const override;
			virtual bool Deserialize(const std::string& serialized) override;

			inline ObjectID ObjectID1() const
			{
				return object1.GetObjectID();
			}

			inline void ObjectID1(ObjectID objectID1)
			{
				this->object1 = objectID1;
			}

			inline ObjectType ObjectType2() const
			{
				return object2.GetObjectType();
			}

			inline ObjectID ObjectID2() const
			{
				return object2.GetObjectID();
			}

			LockableItem* GetObject2();

			inline Type GetType() const
			{
				return type;
			}

			inline Priority GetPriority() const
			{
				return priority;
			}

			inline Data GetData() const
			{
				return data;
			}

			inline bool CompareObject2(const ObjectIdentifier& identifier) const
			{
				return object2 == identifier;
			}

			bool Reserve(Logger::Logger* logger, const LocoID locoID) override;
			bool Lock(Logger::Logger* logger, const LocoID locoID) override;
			bool Release(Logger::Logger* logger, const LocoID locoID) override;
			bool Execute(Logger::Logger* logger, const LocoID locoID, const Delay delay);

		private:
			inline ObjectType ObjectType1() const
			{
				return object1.GetObjectType();
			}

			Manager* manager;
			ObjectIdentifier object1;
			ObjectIdentifier object2;
			Type type;
			Priority priority;
			Data data;
	};
} // namespace DataModel

