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

#include <map>

#include "DataTypes.h"

namespace Hardware
{
	class Z21TurnoutCacheEntry
	{
		public:
			Z21TurnoutCacheEntry()
			:	protocol(ProtocolNone)
			{}

			Z21TurnoutCacheEntry(const protocol_t protocol)
			:	protocol(protocol)
			{}

			protocol_t protocol;
	};

	class Z21TurnoutCache
	{
		public:
			void SetProtocol(const address_t address, const protocol_t protocol)
			{
				Z21TurnoutCacheEntry entry(protocol);
				cache[address] = entry;
			}

			protocol_t GetProtocol(const address_t address)
			{
				if (cache.count(address) == 0)
				{
					return ProtocolDCC;
				}
				return cache[address].protocol;
			}

		private:
			std::map<address_t, Z21TurnoutCacheEntry> cache;
	};
} // namespace

