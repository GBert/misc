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
			inline LocoCacheEntry()
			:	locoID(LocoNone),
				name(""),
				protocol(ProtocolNone),
				address(AddressNone)
			{
				memset(&functionTypes, 0, sizeof(functionTypes));
				memset(&functionIcons, 0, sizeof(functionIcons));
				memset(&functionTimers, 0, sizeof(functionTimers));
			}

			inline LocoID GetLocoID() const
			{
				return locoID;
			}

			inline void SetLocoID(const LocoID locoID)
			{
				this->locoID = locoID;
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

		private:
			static const uint8_t MaxFunctions = 32;
			static const uint8_t MaxFunctionsIncludingZero = MaxFunctions + 1;
			LocoID locoID;
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
			inline LocoCache() {}
			LocoCache(const LocoCache& rhs) = delete;
			LocoCache& operator= (const LocoCache& rhs) = delete;

			inline void Insert(const LocoCacheEntry& entry)
			{
				entries[entry.GetName()] = entry;
			}

			inline void Replace(const LocoCacheEntry& entry, const std::string& oldName)
			{
				Delete(oldName);
				Insert(entry);
			}

			inline LocoCacheEntry Get(const std::string& name)
			{
				return entries.count(name) == 0 ? LocoCacheEntry() : entries.at(name);
			}

			inline void Delete(const std::string& name)
			{
				entries.erase(name);
			}

		private:
			std::map<std::string,LocoCacheEntry> entries;
	};
} // namespace Hardware

