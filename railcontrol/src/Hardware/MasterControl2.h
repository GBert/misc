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

#include "Hardware/Protocols/P50xEthernet.h"
#include "Languages.h"

namespace Hardware
{
	class HardwareParams;

	class MasterControl2 : public Protocols::P50xEthernet
	{
		public:
			MasterControl2() = delete;
			MasterControl2(const MasterControl2&) = delete;
			MasterControl2& operator=(const MasterControl2&) = delete;

			inline MasterControl2(const HardwareParams* params)
			:	Protocols::P50xEthernet(params, "MasterControl2", TypeTams)
			{
				Init();
			}

			virtual ~MasterControl2()
			{
			}

			static inline void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeIpAddress;
				argumentTypes[2] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintMasterControl2);
			}

		private:
			static const unsigned char MaxS88ModulesMasterControl = 104;
	};
} // namespace

