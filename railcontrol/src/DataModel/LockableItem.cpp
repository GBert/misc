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

#include "DataModel/LockableItem.h"
#include "DataModel/Object.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	string LockableItem::Serialize() const
	{
		return "lockState=" + std::to_string(lockState) + ";locoID=" + std::to_string(locoID);

	}

	bool LockableItem::Deserialize(const map<string, string> arguments)
	{
		locoID = Utils::Utils::GetIntegerMapEntry(arguments, "locoID", LocoNone);
		lockState = static_cast<LockState>(Utils::Utils::GetIntegerMapEntry(arguments, "lockState", LockStateFree));
		return true;
	}

	bool LockableItem::Reserve(Logger::Logger* logger, const LocoID locoID)
	{
		std::lock_guard<std::mutex> Guard(lockMutex);
		if (this->locoID == locoID)
		{
			if (lockState == LockStateFree)
			{
				lockState = LockStateReserved;
			}
			return true;
		}

		if (this->locoID != LocoNone)
		{
			Object *object = dynamic_cast<Object*>(this);
			if (object == nullptr)
			{
				return false;
			}

			logger->Debug(Languages::TextIsNotFree, object->GetName());
			return false;
		}

		if (lockState != LockStateFree)
		{
			Object *object = dynamic_cast<Object*>(this);
			if (object == nullptr)
			{
				return false;
			}

			logger->Debug(Languages::TextIsNotFree, object->GetName());
			return false;
		}
		lockState = LockStateReserved;
		this->locoID = locoID;
		return true;
	}

	bool LockableItem::Lock(Logger::Logger* logger, const LocoID locoID)
	{
		std::lock_guard<std::mutex> Guard(lockMutex);

		if (this->locoID != locoID)
		{
			Object *object = dynamic_cast<Object*>(this);
			if (object == nullptr)
			{
				return false;
			}

			logger->Debug(Languages::TextIsNotFree, object->GetName());
			return false;
		}

		if (lockState != LockStateReserved && lockState != LockStateHardLocked)
		{
			Object *object = dynamic_cast<Object*>(this);
			if (object == nullptr)
			{
				return false;
			}

			logger->Debug(Languages::TextIsNotFree, object->GetName());
			return false;
		}

		lockState = LockStateHardLocked;
		return true;
	}

	bool LockableItem::Release(__attribute__((unused)) Logger::Logger* logger, const LocoID locoID)
	{
		std::lock_guard<std::mutex> Guard(lockMutex);
		if (this->locoID != locoID && locoID != LocoNone)
		{
			return false;
		}
		this->locoID = LocoNone;
		lockState = LockStateFree;
		return true;
	}

} // namespace DataModel

