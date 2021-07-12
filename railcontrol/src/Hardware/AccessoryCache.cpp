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

#include "Hardware/AccessoryCache.h"
#include "Manager.h"

namespace Hardware
{
	void AccessoryCache::Save(AccessoryCacheEntry& entry)
	{
		const std::string& matchKey = entry.GetMatchKey();
		UpdateData(entry, matchKey);
		entries.emplace(matchKey, entry);
	}

	void AccessoryCache::UpdateData(AccessoryCacheEntry& entry, const std::string& matchKey)
	{
		DataModel::Accessory* accessory = manager->GetAccessoryByMatchKey(GetControlId(), matchKey);
		if (accessory != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeAccessory, accessory->GetID()));
			*accessory = entry;
			return;
		}
		DataModel::Signal* signal = manager->GetSignalByMatchKey(GetControlId(), matchKey);
		if (signal != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSignal, signal->GetID()));
			*signal = entry;
			return;
		}
		DataModel::Switch* mySwitch = manager->GetSwitchByMatchKey(GetControlId(), matchKey);
		if (mySwitch != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeSwitch, mySwitch->GetID()));
			*mySwitch = entry;
			return;
		}
	}

	DataModel::ObjectIdentifier AccessoryCache::Delete(const std::string& matchKey)
	{
		const DataModel::ObjectIdentifier objectIdentifier = Get(matchKey).GetObjectIdentifier();
		entries.erase(matchKey);
		const ObjectType objectType = objectIdentifier.GetObjectType();
		switch (objectType)
		{
			case ObjectTypeAccessory:
			{
				const AccessoryID accessoryId = objectIdentifier.GetObjectID();
				manager->AccessoryRemoveMatchKey(accessoryId);
				return DataModel::ObjectIdentifier(ObjectTypeAccessory, accessoryId);
			}

			case ObjectTypeSignal:
			{
				const SignalID signalId = objectIdentifier.GetObjectID();
				manager->SignalRemoveMatchKey(signalId);
				return DataModel::ObjectIdentifier(ObjectTypeSignal, signalId);
			}

			case ObjectTypeSwitch:
			{
				const SwitchID switchId = objectIdentifier.GetObjectID();
				manager->SwitchRemoveMatchKey(switchId);
				return DataModel::ObjectIdentifier(ObjectTypeSwitch, switchId);
			}

			default:
				return DataModel::ObjectIdentifier();
		}
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

