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

#include "Hardware/CS2Udp.h"
#include "Utils/Utils.h"

namespace Hardware
{
	CS2Udp::CS2Udp(const HardwareParams* params)
	:	ProtocolMaerklinCAN(params,
			Logger::Logger::GetLogger("CS2UDP " + params->GetName() + " " + params->GetArg1()),
			"Maerklin Central Station 2 (CS2) UDP / " + params->GetName() + " at IP " + params->GetArg1(),
			params->GetName()),
	 	senderConnection(logger, params->GetArg1(), CS2SenderPort),
	 	receiverConnection(logger, "0.0.0.0", CS2ReceiverPort)
	{
		logger->Info(Languages::TextStarting, GetFullName());

		if (senderConnection.IsConnected())
		{
			logger->Info(Languages::TextSenderSocketCreated);
		}
		else
		{
			logger->Error(Languages::TextUnableToCreateUdpSocketForSendingData);
		}

		Init();
	}

	CS2Udp::~CS2Udp()
	{
		receiverConnection.Terminate();
		logger->Info(Languages::TextTerminatingSenderSocket);
	}

	void CS2Udp::Send(const unsigned char* buffer)
	{
		if (senderConnection.Send(buffer, CANCommandBufferLength) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2Udp::Receiver()
	{
		Utils::Utils::SetThreadName("CS2Udp");
		logger->Info(Languages::TextReceiverThreadStarted);
		if (!receiverConnection.IsConnected())
		{
			logger->Error(Languages::TextUnableToCreateUdpSocketForReceivingData);
			return;
		}

		bool ret = receiverConnection.Bind();
		if (!ret)
		{
			logger->Error(Languages::TextUnableToBindUdpSocket);
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		while(run)
		{
			ssize_t datalen = receiverConnection.Receive(buffer, sizeof(buffer));
			if (!run)
			{
				break;
			}

			if (datalen < 0)
			{
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
		receiverConnection.Terminate();
		logger->Info(Languages::TextTerminatingReceiverThread);
	}
} // namespace
