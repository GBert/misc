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
#include "DataModel/ObjectIdentifier.h"

class Manager;

namespace Hardware
{
	class AccessoryCacheEntry
	{
			AccessoryCacheEntry() = delete;

		public:
			inline AccessoryCacheEntry(const ControlID controlId)
			:	controlId(controlId),
				protocol(ProtocolNone),
				address(AddressNone),
				matchKey("")
			{
			}

			inline ControlID GetControlID() const
			{
				return controlId;
			}

			inline DataModel::ObjectIdentifier GetObjectIdentifier() const
			{
				return objectIdentifier;
			}

			inline void SetObjectIdentifier(const DataModel::ObjectIdentifier objectIdentifier)
			{
				this->objectIdentifier = objectIdentifier;
			}

			inline void ClearObjectIdentifier()
			{
				this->objectIdentifier.Clear();
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

			inline const std::string& GetMatchKey() const
			{
				return matchKey;
			}

			inline void SetMatchKey(const std::string& matchKey)
			{
				this->matchKey = matchKey;
			}

			inline void SetMatchKey(const unsigned int matchKey)
			{
				this->matchKey = std::to_string(matchKey);
			}

		private:
			const ControlID controlId;
			DataModel::ObjectIdentifier objectIdentifier;
			std::string name;
			Protocol protocol;
			Address address;
			std::string matchKey;
	};

	class AccessoryCache
	{
			AccessoryCache() = delete;
			AccessoryCache(const AccessoryCache& rhs) = delete;
			AccessoryCache& operator= (const AccessoryCache& rhs) = delete;

		public:
			inline AccessoryCache(const ControlID controlId,
				Manager* const manager)
			:	controlId(controlId),
				manager(manager)
			{
			}

			inline ControlID GetControlId() const
			{
				return controlId;
			}

			inline void Save(AccessoryCacheEntry& entry)
			{
				const std::string& oldMatchKey = entry.GetMatchKey();
				Save(entry, oldMatchKey);
			}

			void Save(AccessoryCacheEntry& entry, const std::string& oldMatchKey);

			DataModel::ObjectIdentifier Delete(const std::string& matchKey);

			inline const AccessoryCacheEntry Get(const std::string& matchKey) const
			{
				return entries.count(matchKey) == 0 ? AccessoryCacheEntry(controlId) : entries.at(matchKey);
			}

			inline const std::map<std::string,AccessoryCacheEntry>& GetAll() const
			{
				return entries;
			}

			void SetObjectIdentifier(const DataModel::ObjectIdentifier objectIdentifier, const std::string& matchKey);

		private:
			bool UpdateData1(AccessoryCacheEntry& entry, const std::string& matchKey);
			bool UpdateData2(AccessoryCacheEntry& entry, const std::string& matchKey);

			const ControlID controlId;
			Manager* const manager;
			std::map<std::string,AccessoryCacheEntry> entries;
	};
} // namespace Hardware

