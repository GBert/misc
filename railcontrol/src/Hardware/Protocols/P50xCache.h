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

namespace Hardware
{
	namespace Protocols
	{
		class P50xCacheEntry
		{
			public:
				inline P50xCacheEntry()
				:	speed(0),
					orientationF0(0),
					functions(0)
				{
				}

				unsigned char speed;
				unsigned char orientationF0;
				union
				{
					uint32_t functions;
					unsigned char function[4];
				};
		};

		class P50xCache
		{
			public:
				P50xCache(const P50xCache&) = delete;
				P50xCache& operator=(const P50xCache&) = delete;

				inline P50xCache()
				{
				}

				void SetSpeed(const Address address, const Speed speed)
				{
					P50xCacheEntry entry = GetData(address);

					if (speed == 0)
					{
						entry.speed = 0;
					}
					else if (speed > 1000)
					{
						entry.speed = 127;
					}
					else
					{
						entry.speed = (speed >> 3) + 2;
					}

					cache[address] = entry;
				}

				void SetOrientation(const Address address, const Orientation orientation)
				{
					P50xCacheEntry entry = GetData(address);

					entry.orientationF0 &= ~(1 << 5);
					entry.orientationF0 |= static_cast<unsigned char>(orientation) << 5;

					cache[address] = entry;
				}

				void SetFunction(const Address address,
				    const DataModel::LocoFunctionNr function,
				    const DataModel::LocoFunctionState on)
				{
					bool onInternal = static_cast<bool>(on);
					P50xCacheEntry entry = GetData(address);

					if (function == 0)
					{
						entry.orientationF0 &= ~(1 << 4);
						entry.orientationF0 |= static_cast<unsigned char>(onInternal) << 4;
					}
					else
					{
						unsigned char shift = function - 1;
						entry.functions &= ~(1 << shift);
						entry.functions |= static_cast<uint32_t>(onInternal) << shift;
					}

					cache[address] = entry;
				}

				inline P50xCacheEntry GetData(const Address address) const
				{
					return cache.count(address) == 0 ? P50xCacheEntry() : cache.at(address);
				}

			private:
				std::map<Address, P50xCacheEntry> cache;
		};
	} // namespace
} // namespace

