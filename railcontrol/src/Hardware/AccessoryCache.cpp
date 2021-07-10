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
		DataModel::Accessory* accessory = manager->GetAccessoryByMatchKey(GetControlId(), matchKey);
		if (accessory != nullptr)
		{
			entry.SetObjectIdentifier(DataModel::ObjectIdentifier(ObjectTypeAccessory, accessory->GetID()));
			*accessory = entry;
		}
		entries.emplace(matchKey, entry);
	}

	AccessoryID AccessoryCache::Delete(const std::string& matchKey)
	{
		const DataModel::ObjectIdentifier objectIdentifier = Get(matchKey).GetObjectIdentifier();
		const AccessoryID accessoryId = objectIdentifier.GetObjectID();
		manager->AccessoryRemoveMatchKey(accessoryId);
		entries.erase(matchKey);
		return accessoryId;
	}

	void AccessoryCache::SetObjectIdentifier(const DataModel::ObjectIdentifier objectIdentifier, const std::string& matchKey)
	{
		for (auto& accessoryCacheEntry : entries)
		{
			AccessoryCacheEntry& entry = accessoryCacheEntry.second;
			if (entry.GetObjectIdentifier() == objectIdentifier)
			{
				entry.ClearObjectIdentifier();
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

