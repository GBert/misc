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
	class ProtocolZ21FeedbackCache
	{
		public:
			ProtocolZ21FeedbackCache(const ProtocolZ21FeedbackCache&) = delete;
			ProtocolZ21FeedbackCache& operator=(const ProtocolZ21FeedbackCache&) = delete;

			inline ProtocolZ21FeedbackCache()
			{
				for (unsigned char module = 0; module < MaxModules; ++module)
				{
					cache[module] = 0;
				}
			}

			inline void Set(const unsigned char module, const unsigned char data)
			{
				if (module >= MaxModules)
				{
					return;
				}
				cache[module] = data;
			}

			inline unsigned char Get(unsigned char module)
			{
				if (module >= MaxModules)
				{
					return 0;
				}
				return cache[module];
			}

		private:
			static const unsigned char MaxModules = 20;
			unsigned char cache[MaxModules];
	};
} // namespace

