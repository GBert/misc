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

#include "Hardware/LocoCache.h"
#include "Manager.h"

namespace Hardware
{
	void LocoCache::Save(LocoCacheEntry& entry, const std::string& oldMatchKey)
	{
		const std::string& matchKey = entry.GetMatchKey();
		DataModel::Loco* loco = nullptr;

		bool matchKeyChanged = matchKey.compare(oldMatchKey) != 0;
		if (matchKeyChanged)
		{
			LocoID locoId = Delete(oldMatchKey);
			loco = manager->GetLoco(locoId);
		}

		if (loco == nullptr)
		{
			loco = manager->GetLocoByMatchKey(GetControlId(), oldMatchKey);
		}

		if (loco == nullptr && matchKeyChanged)
		{
			loco = manager->GetLocoByMatchKey(GetControlId(), matchKey);
		}

		if (loco != nullptr)
		{
			entry.SetLocoID(loco->GetID());
			*loco = entry;
		}
		entries.emplace(matchKey, entry);
	}

	LocoID LocoCache::Delete(const std::string& matchKey)
	{
		LocoID locoId = Get(matchKey).GetLocoID();
		manager->LocoRemoveMatchKey(locoId);
		entries.erase(matchKey);
		return locoId;
	}

	void LocoCache::SetLocoId(const LocoID locoId, const std::string& matchKey)
	{
		for (auto& locoCacheEntry : entries)
		{
			LocoCacheEntry& entry = locoCacheEntry.second;
			if (entry.GetLocoID() == locoId)
			{
				entry.SetLocoID(LocoNone);
			}
		}
		auto entry = entries.find(matchKey);
		if (entry == entries.end())
		{
			return;
		}
		entry->second.SetLocoID(locoId);
	}
} // namespace Hardware

