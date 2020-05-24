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

#include <mutex>
#include <string>

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class Hsi88 : HardwareInterface
	{
		public:
			Hsi88(const HardwareParams* params);
			~Hsi88();

			bool CanHandleFeedback() const override { return true; }

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				argumentTypes[2] = ArgumentTypeS88Modules;
				argumentTypes[3] = ArgumentTypeS88Modules;
				argumentTypes[4] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintHsi88);
			}

		private:
			static const unsigned char MaxS88Modules = 62;

			Logger::Logger* logger;
			Network::Serial serialLine;
			volatile bool run;
			unsigned char s88Modules1;
			unsigned char s88Modules2;
			unsigned char s88Modules3;
			unsigned short s88Modules;

			std::thread checkEventsThread;
			unsigned char s88Init[MaxS88Modules];
			unsigned char s88Memory[MaxS88Modules];

			std::string ReadUntilCR();
			std::string GetVersion();
			unsigned char ConfigureS88();
			void ReadData();
			void CheckFeedbackByte(const unsigned char dataByte, const unsigned char module);

			void CheckEventsWorker();
	};

	extern "C" Hsi88* create_Hsi88(const HardwareParams* params);
	extern "C" void destroy_Hsi88(Hsi88* opendcc);

} // namespace

