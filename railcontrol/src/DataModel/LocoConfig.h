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

#include "DataModel/Loco.h"
#include "Hardware/LocoCache.h"

namespace DataModel
{
	class LocoConfig
	{
		public:
			inline LocoConfig()
			:	controlId(ControlNone),
				locoId(LocoNone),
				address(AddressDefault),
				protocol(ProtocolNone),
				isInUse(false)
			{
			}

			inline LocoConfig(const DataModel::Loco& loco)
			:	controlId(loco.GetControlID()),
				locoId(loco.GetID()),
				address(loco.GetAddress()),
				protocol(loco.GetProtocol()),
				name(loco.GetName()),
				matchKey(loco.GetMatchKey()),
				isInUse(loco.IsInUse())
			{
				ConfigureFunctions(loco.GetFunctionStates());
			}

			inline LocoConfig(const Hardware::LocoCacheEntry& loco)
			:	controlId(loco.GetControlID()),
				locoId(loco.GetLocoID()),
				address(loco.GetAddress()),
				protocol(loco.GetProtocol()),
				name(loco.GetName()),
				matchKey(loco.GetMatchKey()),
				isInUse(false)
			{
				ConfigureFunctions(loco.GetFunctionStates());
			}

			inline LocoConfig& operator=(const DataModel::Loco& loco)
			{
				controlId = loco.GetControlID();
				locoId = loco.GetID();
				address = loco.GetAddress();
				protocol = loco.GetProtocol();
				name = loco.GetName();
				matchKey = loco.GetMatchKey();
				isInUse = loco.IsInUse();
				ConfigureFunctions(loco.GetFunctionStates());
				return *this;
			}

			inline LocoConfig& operator=(const Hardware::LocoCacheEntry& loco)
			{
				controlId = loco.GetControlID();
				locoId = loco.GetLocoID();
				address = loco.GetAddress();
				protocol = loco.GetProtocol();
				name = loco.GetName();
				matchKey = loco.GetMatchKey();
				ConfigureFunctions(loco.GetFunctionStates());
				return *this;
			}

			inline ControlID GetControlId() const
			{
				return controlId;
			}

			inline LocoID GetLocoId() const
			{
				return locoId;
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

			inline void GetFunctions(LocoFunctionEntry* out) const
			{
				functions.GetFunctions(out);
			}

			inline void ConfigureFunctions(const std::vector<LocoFunctionEntry>& newEntries)
			{
				functions.ConfigureFunctions(newEntries);
			}

		private:
			ControlID controlId;
			LocoID locoId;
			Address address;
			Protocol protocol;
			std::string name;
			std::string matchKey;
			bool isInUse;
			LocoFunctions functions;
	};
} // namespace DataModel
