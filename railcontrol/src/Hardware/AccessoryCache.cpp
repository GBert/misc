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

#include "DataModel/Accessory.h"
#include "DataModel/ObjectIdentifier.h"
#include "DataModel/Signal.h"
#include "DataModel/Switch.h"
#include "Hardware/AccessoryCache.h"
#include "Manager.h"

namespace Hardware
{
	void AccessoryCache::Save(AccessoryCacheEntry& entry, const std::string& oldMatchKey)
	{
		const std::string& matchKey = entry.GetMatchKey();

		bool ret = false;
		const bool matchKeyChanged = matchKey.compare(oldMatchKey) != 0;
		if (matchKeyChanged)
		{
			ret = UpdateData1(entry, oldMatchKey);
		}

		if (!ret)
		{
			ret = UpdateData2(entry, oldMatchKey);
		}

		if (!ret && matchKeyChanged)
		{
			ret = UpdateData2(entry, matchKey);
		}
		entries.emplace(matchKey, entry);
	}

	bool AccessoryCache::UpdateData1(AccessoryCacheEntry& entry, const std::string& matchKey)
	{
		const DataModel::ObjectIdentifier objectIdentifier = Delete(matchKey);
		switch (objectIdentifier.GetObjectType())
		{
			case ObjectTypeAccessory:
			{
				DataModel::Accessory* accessory = manager->GetAccessory(objectIdentifier.GetObjectID());
				if (accessory)
				{
					entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeAccessory, accessory->GetID()));
					*accessory = entry;
					return true;
				}
				return false;
			}

			case ObjectTypeSignal:
			{
				DataModel::Signal* signal = manager->GetSignal(objectIdentifier.GetObjectID());
				if (signal)
				{
					entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSignal, signal->GetID()));
					*signal = entry;
					return true;
				}
				return false;
			}

			case ObjectTypeSwitch:
			{
				DataModel::Switch* mySwitch = manager->GetSwitch(objectIdentifier.GetObjectID());
				if (mySwitch)
				{
					entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSwitch, mySwitch->GetID()));
					*mySwitch = entry;
					return true;
				}
				return false;
			}

			default:
				return false;
		}
	}

	bool AccessoryCache::UpdateData2(AccessoryCacheEntry& entry, const std::string& matchKey)
	{
		DataModel::Accessory* accessory = manager->GetAccessoryByMatchKey(GetControlId(), matchKey);
		if (accessory != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeAccessory, accessory->GetID()));
			*accessory = entry;
			return true;
		}
		DataModel::Signal* signal = manager->GetSignalByMatchKey(GetControlId(), matchKey);
		if (signal != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSignal, signal->GetID()));
			*signal = entry;
			return true;
		}
		DataModel::Switch* mySwitch = manager->GetSwitchByMatchKey(GetControlId(), matchKey);
		if (mySwitch != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSwitch, mySwitch->GetID()));
			*mySwitch = entry;
			return true;
		}
		return false;
	}

	DataModel::ObjectIdentifier AccessoryCache::Delete(const std::string& matchKey)
	{
		DataModel::ObjectIdentifier objectIdentifier = Get(matchKey).GetObjectIdentifier();
		entries.erase(matchKey);
		const ObjectType objectType = objectIdentifier.GetObjectType();
		switch (objectType)
		{
			case ObjectTypeAccessory:
			{
				const AccessoryID accessoryId = objectIdentifier.GetObjectID();
				manager->AccessoryRemoveMatchKey(accessoryId);
				objectIdentifier.SetObjectID(accessoryId);
				break;
			}

			case ObjectTypeSignal:
			{
				const SignalID signalId = objectIdentifier.GetObjectID();
				manager->SignalRemoveMatchKey(signalId);
				objectIdentifier.SetObjectID(signalId);
				break;
			}

			case ObjectTypeSwitch:
			{
				const SwitchID switchId = objectIdentifier.GetObjectID();
				manager->SwitchRemoveMatchKey(switchId);
				objectIdentifier.SetObjectID(switchId);
				break;
			}

			default:
				return DataModel::ObjectIdentifier();
		}
		return objectIdentifier;
	}

	void AccessoryCache::SetObjectIdentifier(const DataModel::ObjectIdentifier objectIdentifier, const std::string& matchKey)
	{
		if (objectIdentifier.IsSet())
		{
			for (auto& accessoryCacheEntry : entries)
			{
				AccessoryCacheEntry& entry = accessoryCacheEntry.second;
				if (entry.GetObjectIdentifier() == objectIdentifier)
				{
					entry.ClearObjectIdentifier();
				}
			}
		}
		auto entry = entries.find(matchKey);
		if (entry == entries.end())
		{
			return;
		}
		entry->second.SetObjectIdentifier(objectIdentifier);
	}
} // namespace Hardware

