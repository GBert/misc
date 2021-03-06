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
	void LocoCache::InsertByName(LocoCacheEntry& entry)
	{
		const std::string& name = entry.GetName();
		entry.SetLocoID(manager->GetLocoIdByMatchKey(GetControlId(), name));
		entries.emplace(name, entry);
	}

	void LocoCache::DeleteByName(const std::string& name)
	{
		manager->LocoRemoveMatchKey(GetByName(name).GetLocoID());
		entries.erase(name);
	}

	void LocoCache::SetLocoIdByName(const LocoID locoId, const std::string& name)
	{
		for (auto& locoCacheEntry : entries)
		{
			LocoCacheEntry& entry = locoCacheEntry.second;
			if (entry.GetLocoID() == locoId)
			{
				entry.SetLocoID(LocoNone);
			}
		}
		auto entry = entries.find(name);
		if (entry == entries.end())
		{
			return;
		}
		entry->second.SetLocoID(locoId);
	}
} // namespace Hardware

