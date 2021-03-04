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

#include "Hardware/CS2Tcp.h"
#include "Utils/Utils.h"

namespace Hardware
{
	CS2Tcp::CS2Tcp(const HardwareParams* params)
	:	ProtocolMaerklinCAN(params,
			Logger::Logger::GetLogger("CS2TCP " + params->GetName() + " " + params->GetArg1()),
			"Maerklin Central Station 2 (CS2) TCP / " + params->GetName() + " at IP " + params->GetArg1(),
			params->GetName()),
	 	connection(Network::TcpClient::GetTcpClientConnection(logger, params->GetArg1(), CS2Port))
	{
		logger->Info(Languages::TextStarting, GetFullName());

		if (connection.IsConnected() == false)
		{
			logger->Error(Languages::TextUnableToCreateTcpSocket);
			return;
		}
		Init();
	}

	void CS2Tcp::Send(const unsigned char* buffer)
	{
		if (connection.Send(buffer, CANCommandBufferLength) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2Tcp::Receiver()
	{
		run = true;
		Utils::Utils::SetThreadName("CS2Tcp");
		logger->Info(Languages::TextReceiverThreadStarted);
		if (connection.IsConnected() == false)
		{
			logger->Error(Languages::TextUnableToCreateUdpSocketForReceivingData);
			return;
		}

		unsigned char buffer[CANCommandBufferLength];
		while(run)
		{
			ssize_t datalen = connection.Receive(buffer, sizeof(buffer));
			if (run == false)
			{
				break;
			}

			if (datalen < 0)
			{
				if (errno == ETIMEDOUT)
				{
					continue;
				}
				logger->Error(Languages::TextUnableToReceiveData);
				break;
			}

			if (datalen != 13)
			{
				logger->Error(Languages::TextInvalidDataReceived);
				continue;
			}
			Parse(buffer);
		}
		connection.Terminate();
		logger->Info(Languages::TextTerminatingReceiverThread);
	}
} // namespace
