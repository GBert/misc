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
#include "Hardware/ProtocolMaerklinCAN.h"
#include "Logger/Logger.h"
#include "Network/UdpConnection.h"

namespace Hardware
{
	class CS2Udp : protected ProtocolMaerklinCAN
	{
		public:
			CS2Udp(const HardwareParams* params);
			~CS2Udp();

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeIpAddress;
				hint = Languages::GetText(Languages::TextHintCs2Udp);
			}

		private:
			Network::UdpConnection senderConnection;
			Network::UdpConnection receiverConnection;

			void Send(const unsigned char* buffer) override;
			void Receiver() override;

			static const unsigned short CS2SenderPort = 15731;
			static const unsigned short CS2ReceiverPort = 15730;
	};

} // namespace

