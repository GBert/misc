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

namespace Hardware
{
	class OpenDccCacheEntry
	{
		public:
			OpenDccCacheEntry()
			:	speed(0),
			 	directionF0(0),
			 	functions(0)
			{}

			unsigned char speed;
			unsigned char directionF0;
			union
			{
					uint32_t functions;
					unsigned char function[4];
			};
	};

	class OpenDccCache
	{
		public:
			OpenDccCache() {};
			~OpenDccCache() {};

			void SetSpeed(const Address address, const Speed speed)
			{
				OpenDccCacheEntry entry = GetData(address);

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

			void SetDirection(const Address address, const Direction direction)
			{
				OpenDccCacheEntry entry = GetData(address);

				entry.directionF0 &= ~(1 << 5);
				entry.directionF0 |= static_cast<unsigned char>(direction) << 5;

				cache[address] = entry;
			}

			void SetFunction(const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on)
			{
				bool onInternal = static_cast<bool>(on);
				OpenDccCacheEntry entry = GetData(address);

				if (function == 0)
				{
					entry.directionF0 &= ~(1 << 4);
					entry.directionF0 |= static_cast<unsigned char>(onInternal) << 4;
				}
				else
				{
					unsigned char shift = function - 1;
					entry.functions &= ~(1 << shift);
					entry.functions |= static_cast<uint32_t>(onInternal) << shift;
				}

				cache[address] = entry;
			}

			OpenDccCacheEntry GetData(const Address address) const
			{
				if (cache.count(address) == 0)
				{
					OpenDccCacheEntry entry;
					return entry;
				}

				return cache.at(address);
			}

		private:
			std::map<Address, OpenDccCacheEntry> cache;
	};
} // namespace

