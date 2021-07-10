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

#include <string>

#include "DataModel/Accessory.h"
#include "Hardware/AccessoryCache.h"

namespace DataModel
{
	class AccessoryConfig
	{
		public:
			inline AccessoryConfig()
			:	controlId(ControlNone),
				address(AddressDefault),
				protocol(ProtocolNone),
				isInUse(false)
			{
			}

			inline AccessoryConfig(const DataModel::Accessory& accessory)
			:	controlId(accessory.GetControlID()),
				objectIdentifier(ObjectTypeAccessory, accessory.GetID()),
				address(accessory.GetAddress()),
				protocol(accessory.GetProtocol()),
				name(accessory.GetName()),
				matchKey(accessory.GetMatchKey()),
				isInUse(accessory.IsInUse())
			{
			}

			inline AccessoryConfig(const Hardware::AccessoryCacheEntry& accessory)
			:	controlId(accessory.GetControlID()),
				objectIdentifier(accessory.GetObjectIdentifier()),
				address(accessory.GetAddress()),
				protocol(accessory.GetProtocol()),
				name(accessory.GetName()),
				matchKey(accessory.GetMatchKey()),
				isInUse(false)
			{
			}

			inline AccessoryConfig& operator=(const DataModel::Accessory& accessory)
			{
				controlId = accessory.GetControlID();
				objectIdentifier = ObjectIdentifier(ObjectTypeAccessory, accessory.GetID());
				address = accessory.GetAddress();
				protocol = accessory.GetProtocol();
				name = accessory.GetName();
				matchKey = accessory.GetMatchKey();
				isInUse = accessory.IsInUse();
				return *this;
			}

			inline AccessoryConfig& operator=(const Hardware::AccessoryCacheEntry& accessory)
			{
				controlId = accessory.GetControlID();
				objectIdentifier = accessory.GetObjectIdentifier();
				address = accessory.GetAddress();
				protocol = accessory.GetProtocol();
				name = accessory.GetName();
				matchKey = accessory.GetMatchKey();
				return *this;
			}

			inline ControlID GetControlId() const
			{
				return controlId;
			}

			inline ObjectIdentifier GetObjectIdentifier() const
			{
				return objectIdentifier;
			}

			inline Address GetAddress() const
			{
				return address;
			}

			inline Protocol GetProtocol() const
			{
				return protocol;
			}

			inline std::string GetName() const
			{
				return name;
			}

			inline void SetName(const std::string& name)
			{
				this->name = name;
			}

			inline std::string GetMatchKey() const
			{
				return matchKey;
			}

			inline bool IsInUse() const
			{
				return isInUse;
			}

		private:
			ControlID controlId;
			ObjectIdentifier objectIdentifier;
			Address address;
			Protocol protocol;
			std::string name;
			std::string matchKey;
			bool isInUse;
	};
} // namespace DataModel
