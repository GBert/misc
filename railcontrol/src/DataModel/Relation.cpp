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
#include <sstream>
#include <string>

#include "DataModel/Relation.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Relation::Serialize() const
	{
		stringstream ss;
		ss << LockableItem::Serialize()
			<< ";type=" << static_cast<int>(type)
			<< ";objectType1=" << static_cast<int>(objectType1)
			<< ";objectID1=" << objectID1
			<< ";objectType2=" << static_cast<int>(objectType2)
			<< ";objectID2=" << objectID2
			<< ";priority=" << static_cast<int>(priority)
			<< ";data=" << static_cast<int>(data);
		return ss.str();
	}

	bool Relation::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		LockableItem::Deserialize(arguments);
		objectType1 = static_cast<ObjectType>(Utils::Utils::GetIntegerMapEntry(arguments, "objectType1"));
		type = static_cast<Type>(Utils::Utils::GetIntegerMapEntry(arguments, "type", objectType1 << 3));
		objectID1 = Utils::Utils::GetIntegerMapEntry(arguments, "objectID1");
		objectType2 = static_cast<ObjectType>(Utils::Utils::GetIntegerMapEntry(arguments, "objectType2"));
		objectID2 = Utils::Utils::GetIntegerMapEntry(arguments, "objectID2");
		priority = Utils::Utils::GetIntegerMapEntry(arguments, "priority");
		data = Utils::Utils::GetIntegerMapEntry(arguments, "accessoryState"); // FIXME: remove later
		data = Utils::Utils::GetIntegerMapEntry(arguments, "data", data);
		return true;
	}

	bool Relation::Execute(Logger::Logger* logger, const LocoID locoID, const Delay delay)
	{
		switch (objectType2)
		{
			case ObjectTypeAccessory:
			{
				bool ret = manager->AccessoryState(ControlTypeInternal, objectID2, static_cast<AccessoryState>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeSwitch:
			{
				bool ret = manager->SwitchState(ControlTypeInternal, objectID2, static_cast<AccessoryState>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeSignal:
			{
				bool ret = manager->SignalState(ControlTypeInternal, objectID2, static_cast<AccessoryState>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeTrack:
				manager->TrackSetLocoDirection(objectID2, static_cast<Direction>(data));
				return true;


			case ObjectTypeStreet:
				return manager->StreetExecute(logger, locoID, objectID2);

			case ObjectTypeLoco:
				manager->LocoFunction(ControlTypeInternal, locoID, static_cast<Function>(objectID2), static_cast<LocoFunctions::FunctionState>(data));
				return true;

			default:
				return false;
		}
		Utils::Utils::SleepForMilliseconds(delay);
		return true;
	}

	LockableItem* Relation::GetObject2()
	{
		switch (objectType2)
		{
			case ObjectTypeAccessory:
				return manager->GetAccessory(objectID2);

			case ObjectTypeSwitch:
				return manager->GetSwitch(objectID2);

			case ObjectTypeTrack:
				return manager->GetTrack(objectID2);

			case ObjectTypeSignal:
				return manager->GetSignal(objectID2);

			case ObjectTypeStreet:
				return manager->GetStreet(objectID2);

			default:
				return nullptr;
		}
	}

	bool Relation::Reserve(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = LockableItem::Reserve(logger, locoID);
		if (ret == false)
		{
			logger->Debug(Languages::TextUnableToReserve);
			return false;
		}

		if (ObjectType2() == ObjectTypeLoco)
		{
			return true;
		}

		LockableItem* lockable = GetObject2();
		if (lockable == nullptr)
		{
			logger->Debug(Languages::TextRelationTargetNotFound);
			LockableItem::Release(logger, locoID);
			return false;
		}

		Street* street = dynamic_cast<Street*>(lockable);
		if (street != nullptr)
		{
			return street->Reserve(logger, locoID);
		}

		return lockable->Reserve(logger, locoID);
	}

	bool Relation::Lock(Logger::Logger* logger, const LocoID locoID)
	{
		bool ret = LockableItem::Lock(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		if (ObjectType2() == ObjectTypeLoco)
		{
			return true;
		}

		LockableItem* lockable = GetObject2();
		if (lockable == nullptr)
		{
			LockableItem::Release(logger, locoID);
			return false;
		}

		Street* street = dynamic_cast<Street*>(lockable);
		if (street != nullptr)
		{
			return street->Lock(logger, locoID);
		}

		bool retLockable = lockable->Lock(logger, locoID);
		if (retLockable == true)
		{
			return true;
		}

		Object* object = dynamic_cast<Object*>(lockable);
		if (object == nullptr)
		{
			return false;
		}

		logger->Debug(Languages::TextUnableToLock, object->GetName());
		return false;
	}

	bool Relation::Release(Logger::Logger* logger, const LocoID locoID)
	{
		LockableItem* object = GetObject2();
		if (object != nullptr)
		{
			object->Release(logger, locoID);
		}
		return LockableItem::Release(logger, locoID);
	}
} // namespace DataModel

