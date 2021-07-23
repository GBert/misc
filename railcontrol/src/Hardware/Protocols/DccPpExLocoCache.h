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
	namespace Protocols
	{
		class DccPpExLocoCacheEntry
		{
			public:
				DccPpExLocoCacheEntry()
				:	speed(MinSpeed),
				    orientation(OrientationRight)
				{
				}

				DccPpExLocoCacheEntry(const Speed speed, const Orientation orientation)
				:	speed(speed),
				    orientation(orientation)
				{
				}

				Speed speed;
				Orientation orientation;
		};

		class DccPpExLocoCache
		{
			public:
				DccPpExLocoCache& operator=(const DccPpExLocoCache&) = delete;

				DccPpExLocoCacheEntry GetData(const Address address)
				{
					if (cache.count(address) == 1)
					{
						return cache[address];
					}
					DccPpExLocoCacheEntry entry;
					return entry;
				}

				void SetSpeed(const Address address, const Speed speed)
				{
					DccPpExLocoCacheEntry entry = GetData(address);
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
					DccPpExLocoCacheEntry entry = GetData(address);
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

			private:
				std::map<Address, DccPpExLocoCacheEntry> cache;
		};
	} // namespace
} // namespace

