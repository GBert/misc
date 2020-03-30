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
	class Z21LocoCacheEntry
	{
		public:
			Z21LocoCacheEntry()
			:	speed(MinSpeed),
			 	direction(DirectionRight),
			 	functions(0),
			 	protocol(ProtocolNone)
			{}

			Z21LocoCacheEntry(const locoSpeed_t speed, const direction_t direction, const protocol_t protocol)
			:	speed(speed),
			 	direction(direction),
			 	functions(0),
			 	protocol(protocol)
			{}

			locoSpeed_t speed;
			direction_t direction;
			uint32_t functions;
			protocol_t protocol;
	};

	class Z21LocoCache
	{
		public:
			Z21LocoCacheEntry GetData(const address_t address)
			{
				if (cache.count(address) == 1)
				{
					return cache[address];
				}
				Z21LocoCacheEntry entry;
				return entry;
			}

			void SetSpeed(const address_t address, const locoSpeed_t speed)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.speed = speed;
				cache[address] = entry;
			}

			locoSpeed_t GetSpeed(const address_t address)
			{
				if (cache.count(address) == 0)
				{
					return MinSpeed;
				}
				return cache[address].speed;
			}

			void SetDirection(const address_t address, const direction_t direction)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.direction = direction;
				cache[address] = entry;
			}

			direction_t GetDirection(const address_t address)
			{
				if (cache.count(address) == 0)
				{
					return DirectionRight;
				}
				return cache[address].direction;
			}

			void SetSpeedDirectionProtocol(const address_t address, const locoSpeed_t speed, const direction_t direction, const protocol_t protocol)
			{
				Z21LocoCacheEntry entry(speed, direction, protocol);
				cache[address] = entry;
			}

			void SetFunction(const address_t address, const function_t function, const bool on)
			{
				Z21LocoCacheEntry entry = GetData(address);
				uint32_t mask = ~(1 << function);
				entry.functions &= mask;
				entry.functions |= on << function;
				cache[address] = entry;
			}

			uint32_t GetFunctions(const address_t address)
			{
				if (cache.count(address) == 0)
				{
					return 0;
				}
				return cache[address].functions;
			}

			void SetProtocol(const address_t address, const protocol_t protocol)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.protocol = protocol;
				cache[address] = entry;
			}

			protocol_t GetProtocol(const address_t address)
			{
				if (cache.count(address) == 0)
				{
					return ProtocolNone;
				}
				return cache[address].protocol;
			}

		private:
			std::map<address_t, Z21LocoCacheEntry> cache;
	};
} // namespace

