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

#include "Hardware/HardwareParams.h"
#include "Hardware/Protocols/MaerklinCAN.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class CcSchnitte : Protocols::MaerklinCAN
	{
		public:
			CcSchnitte() = delete;
			CcSchnitte(const CcSchnitte&) = delete;
			CcSchnitte& operator=(const CcSchnitte&) = delete;

			CcSchnitte(const HardwareParams* params);

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				hint = Languages::GetText(Languages::TextHintCcSchnitte);
			}

		private:
			Network::Serial serialLine;

			void Send(const unsigned char* buffer) override;
			void Receiver() override;
	};
} // namespace
