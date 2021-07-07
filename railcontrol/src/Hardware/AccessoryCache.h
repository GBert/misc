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

class Manager;

namespace Hardware
{
	class AccessoryCacheEntry
	{
			AccessoryCacheEntry() = delete;

		public:
			inline AccessoryCacheEntry(const ControlID controlId)
			:	controlId(controlId),
				accessoryId(AccessoryNone),
				protocol(ProtocolNone),
				address(AddressNone),
				matchKey("")
			{
			}

			inline ControlID GetControlID() const
			{
				return controlId;
			}

			inline AccessoryID GetAccessoryID() const
			{
				return accessoryId;
			}

			inline void SetAccessoryID(const AccessoryID accessoryId)
			{
				this->accessoryId = accessoryId;
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
			AccessoryID accessoryId;
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

			void Save(AccessoryCacheEntry& entry);

			AccessoryID Delete(const std::string& matchKey);

			inline const AccessoryCacheEntry Get(const std::string& matchKey) const
			{
				return entries.count(matchKey) == 0 ? AccessoryCacheEntry(controlId) : entries.at(matchKey);
			}

			inline const std::map<std::string,AccessoryCacheEntry>& GetAll() const
			{
				return entries;
			}

			void SetAccessoryId(const AccessoryID accessoryId, const std::string& matckKey);

		private:
			const ControlID controlId;
			Manager* const manager;
			std::map<std::string,AccessoryCacheEntry> entries;
	};
} // namespace Hardware

