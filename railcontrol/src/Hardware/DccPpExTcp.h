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

#include "Hardware/HardwareParams.h"
#include "Hardware/Protocols/DccPpEx.h"
#include "Logger/Logger.h"
#include "Network/TcpClient.h"

namespace Hardware
{
	class DccPpExTcp : protected Protocols::DccPpEx
	{
		public:
			DccPpExTcp() = delete;
			DccPpExTcp(const DccPpExTcp&) = delete;
			DccPpExTcp& operator=(const DccPpExTcp&) = delete;

			DccPpExTcp(const HardwareParams* params);

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeIpAddress;
				hint = Languages::GetText(Languages::TextHintDccPpExTcp);
			}

		private:
			Network::TcpConnection connection;

			void Send(const std::string& buffer) override;

			static const unsigned short Port = 2560;
	};
} // namespace

