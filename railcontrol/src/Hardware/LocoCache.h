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

class Manager;

namespace Hardware
{
	class LocoCacheEntry
	{
		public:
			LocoCacheEntry() = delete;

			inline LocoCacheEntry(const ControlID controlId)
			:	controlId(controlId),
				locoId(LocoNone),
				protocol(ProtocolNone),
				address(AddressNone)
			{
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
				functions.SetFunction(nr, type, icon, timer);
			}

			inline void ClearFunction(const DataModel::LocoFunctionNr nr)
			{
				functions.ClearFunction(nr);
			}

			inline std::vector<DataModel::LocoFunctionEntry> GetFunctionStates() const
			{
				return functions.GetFunctionStates();
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
			DataModel::LocoFunctions functions;
	};

	class LocoCache
	{
		public:
			inline LocoCache(const ControlID controlId,
				Manager* const manager)
			:	controlId(controlId),
				manager(manager)
			{
			}

			LocoCache() = delete;

			LocoCache(const LocoCache& rhs) = delete;

			LocoCache& operator= (const LocoCache& rhs) = delete;

			inline ControlID GetControlId() const
			{
				return controlId;
			}

			void InsertByName(LocoCacheEntry& entry);

			inline void ReplaceByName(LocoCacheEntry& entry, const std::string& oldName)
			{
				DeleteByName(oldName);
				InsertByName(entry);
			}

			inline const LocoCacheEntry GetByName(const std::string& name) const
			{
				return entries.count(name) == 0 ? LocoCacheEntry(controlId) : entries.at(name);
			}

			void DeleteByName(const std::string& name);

			inline const std::map<std::string,LocoCacheEntry>& GetAll() const
			{
				return entries;
			}

			void SetLocoIdByName(const LocoID locoId, const std::string& name);

		private:
			const ControlID controlId;
			Manager* const manager;
			std::map<std::string,LocoCacheEntry> entries;
	};
} // namespace Hardware

