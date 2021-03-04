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

#include <cstring>
#include <string>
#include <map>

#include "DataTypes.h"
#include "DataModel/LocoFunctions.h"

namespace Hardware
{
	class LocoCacheEntry
	{
		public:
			LocoCacheEntry() = delete;

			inline LocoCacheEntry(const ControlID controlId)
			:	controlId(controlId),
				locoId(LocoNone),
				name(""),
				protocol(ProtocolNone),
				address(AddressNone)
			{
				memset(&functionTypes, 0, sizeof(functionTypes));
				memset(&functionIcons, 0, sizeof(functionIcons));
				memset(&functionTimers, 0, sizeof(functionTimers));
			}

			inline ControlID GetControlID() const
			{
				return controlId;
			}

			inline LocoID GetLocoID() const
			{
				return locoId;
			}

			inline void SetLocoID(const LocoID locoId)
			{
				this->locoId = locoId;
			}

			inline const std::string& GetName() const
			{
				return name;
			}

			inline void SetName(const std::string& name)
			{
				this->name = name;
			}

			inline Protocol GetProtocol() const
			{
				return protocol;
			}

			inline void SetProtocol(const Protocol protocol)
			{
				this->protocol = protocol;
			}

			inline Address GetAddress() const
			{
				return address;
			}

			inline void SetAddress(const Address address)
			{
				this->address = address;
			}

			inline void SetFunction(const DataModel::LocoFunctionNr nr,
				const DataModel::LocoFunctionType type,
				const DataModel::LocoFunctionIcon icon,
				const DataModel::LocoFunctionTimer timer)
			{
				if (nr >= MaxFunctionsIncludingZero)
				{
					return;
				}
				functionTypes[nr] = type;
				functionIcons[nr] = icon;
				functionTimers[nr] = timer;
			}

			inline void ClearFunction(const DataModel::LocoFunctionNr nr)
			{
				if (nr >= MaxFunctionsIncludingZero)
				{
					return;
				}
				functionTypes[nr] = DataModel::LocoFunctionTypeNone;
				functionIcons[nr] = DataModel::LocoFunctionIconNone;
				functionTimers[nr] = 0;
			}

			inline DataModel::LocoFunctionType GetFunctionType(const DataModel::LocoFunctionNr nr)
			{
				if (nr >= MaxFunctionsIncludingZero)
				{
					return DataModel::LocoFunctionTypeNone;
				}
				return functionTypes[nr];
			}

			inline DataModel::LocoFunctionIcon GetFunctionIcon(const DataModel::LocoFunctionNr nr)
			{
				if (nr >= MaxFunctionsIncludingZero)
				{
					return DataModel::LocoFunctionIconNone;
				}
				return functionIcons[nr];
			}

			inline DataModel::LocoFunctionTimer GetFunctionTimer(const DataModel::LocoFunctionNr nr)
			{
				if (nr >= MaxFunctionsIncludingZero)
				{
					return 0;
				}
				return functionTimers[nr];
			}

			inline const std::string& GetMatchKey() const
			{
				return name;
			}

		private:
			static const uint8_t MaxFunctions = 32;
			static const uint8_t MaxFunctionsIncludingZero = MaxFunctions + 1;

			const ControlID controlId;
			LocoID locoId;
			std::string name;
			Protocol protocol;
			Address address;
			DataModel::LocoFunctionType functionTypes[MaxFunctionsIncludingZero];
			DataModel::LocoFunctionIcon functionIcons[MaxFunctionsIncludingZero];
			DataModel::LocoFunctionTimer functionTimers[MaxFunctionsIncludingZero];
	};

	class LocoCache
	{
		public:
			inline LocoCache(const ControlID controlId)
			:	controlId(controlId)
			{
			}

			LocoCache() = delete;

			LocoCache(const LocoCache& rhs) = delete;

			LocoCache& operator= (const LocoCache& rhs) = delete;

			inline ControlID GetControlId() const
			{
				return controlId;
			}

			inline void InsertByName(const LocoCacheEntry& entry)
			{
				entries.emplace(entry.GetName(), entry);
			}

			inline void ReplaceByName(const LocoCacheEntry& entry, const std::string& oldName)
			{
				DeleteByName(oldName);
				InsertByName(entry);
			}

			inline const LocoCacheEntry GetByName(const std::string& name) const
			{
				return entries.count(name) == 0 ? LocoCacheEntry(controlId) : entries.at(name);
			}

			inline void DeleteByName(const std::string& name)
			{
				entries.erase(name);
			}

			inline const std::map<std::string,LocoCacheEntry>& GetAll() const
			{
				return entries;
			}

			void SetLocoIdByName(const LocoID locoId, const std::string& name)
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

		private:
			const ControlID controlId;
			std::map<std::string,LocoCacheEntry> entries;
	};
} // namespace Hardware

