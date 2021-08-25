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
#include <string>

#include "Languages.h"
#include "Hardware/Protocols/P50xUhlenbrock.h"

namespace Hardware
{
	class HardwareParams;

	class TwinCenter : public Protocols::P50xUhlenbrock
	{
		public:
			TwinCenter() = delete;
			TwinCenter(const TwinCenter&) = delete;
			TwinCenter& operator=(const TwinCenter&) = delete;

			inline TwinCenter(const HardwareParams* params)
			:	Protocols::P50xUhlenbrock(params, "TwinCenter")
			{
				Init();
			}

			virtual ~TwinCenter()
			{
			}

			static inline void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				argumentTypes[2] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintTwinCenter);
			}
	};
} // namespace

