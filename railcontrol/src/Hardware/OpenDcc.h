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

#include "Hardware/ProtocolP50xSerial.h"
#include "Languages.h"

namespace Hardware
{
	class HardwareParams;

	class OpenDcc : public ProtocolP50xSerial
	{
		public:
			OpenDcc() = delete;
			OpenDcc(const OpenDcc&) = delete;
			OpenDcc& operator=(const OpenDcc&) = delete;

			OpenDcc(const HardwareParams* params);

			virtual ~OpenDcc()
			{
			}

			static inline void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				argumentTypes[2] = ArgumentTypeS88Modules;
				argumentTypes[3] = ArgumentTypeS88Modules;
				argumentTypes[4] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintOpenDcc);
			}

		private:
			unsigned char s88Modules1;
			unsigned char s88Modules2;
			unsigned char s88Modules3;
	};
} // namespace

