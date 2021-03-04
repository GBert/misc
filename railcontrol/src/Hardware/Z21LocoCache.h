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

#include <map>

#include "DataTypes.h"

namespace Hardware
{
	class Z21LocoCacheEntry
	{
		public:
			Z21LocoCacheEntry()
			:	speed(MinSpeed),
			 	orientation(OrientationRight),
			 	functions(0),
			 	protocol(ProtocolNone)
			{}

			Z21LocoCacheEntry(const Speed speed, const Orientation orientation, const Protocol protocol)
			:	speed(speed),
			 	orientation(orientation),
			 	functions(0),
			 	protocol(protocol)
			{}

			Speed speed;
			Orientation orientation;
			uint32_t functions;
			Protocol protocol;
	};

	class Z21LocoCache
	{
		public:
			Z21LocoCacheEntry GetData(const Address address)
			{
				if (cache.count(address) == 1)
				{
					return cache[address];
				}
				Z21LocoCacheEntry entry;
				return entry;
			}

			void SetSpeed(const Address address, const Speed speed)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.speed = speed;
				cache[address] = entry;
			}

			Speed GetSpeed(const Address address)
			{
				if (cache.count(address) == 0)
				{
					return MinSpeed;
				}
				return cache[address].speed;
			}

			void SetOrientation(const Address address, const Orientation orientation)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.orientation = orientation;
				cache[address] = entry;
			}

			Orientation GetOrientation(const Address address)
			{
				if (cache.count(address) == 0)
				{
					return OrientationRight;
				}
				return cache[address].orientation;
			}

			void SetSpeedOrientationProtocol(const Address address, const Speed speed, const Orientation orientation, const Protocol protocol)
			{
				Z21LocoCacheEntry entry(speed, orientation, protocol);
				cache[address] = entry;
			}

			void SetFunction(const Address address,
				const DataModel::LocoFunctionNr function,
				const bool on)
			{
				Z21LocoCacheEntry entry = GetData(address);
				uint32_t mask = ~(1 << function);
				entry.functions &= mask;
				entry.functions |= on << function;
				cache[address] = entry;
			}

			uint32_t GetFunctions(const Address address)
			{
				if (cache.count(address) == 0)
				{
					return 0;
				}
				return cache[address].functions;
			}

			void SetProtocol(const Address address, const Protocol protocol)
			{
				Z21LocoCacheEntry entry = GetData(address);
				entry.protocol = protocol;
				cache[address] = entry;
			}

			Protocol GetProtocol(const Address address)
			{
				if (cache.count(address) == 0)
				{
					return ProtocolNone;
				}
				return cache[address].protocol;
			}

		private:
			std::map<Address, Z21LocoCacheEntry> cache;
	};
} // namespace

