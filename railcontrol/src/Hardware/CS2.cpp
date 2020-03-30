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

#include "Hardware/CS2.h"
#include "Utils/Utils.h"

namespace Hardware
{
	extern "C" CS2* create_CS2(const HardwareParams* params)
	{
		return new CS2(params);
	}

	extern "C" void destroy_CS2(CS2* cs2)
	{
		delete(cs2);
	}

	CS2::CS2(const HardwareParams* params)
	:	MaerklinCAN(params->GetManager(),
			params->GetControlID(),
			Logger::Logger::GetLogger("CS2 " + params->GetName() + " " + params->GetArg1()),
			"Maerklin Central Station 2 (CS2) / " + params->GetName() + " at IP " + params->GetArg1()),
	 	run(true),
	 	senderConnection(logger, params->GetArg1(), CS2SenderPort),
	 	receiverConnection(logger, "0.0.0.0", CS2ReceiverPort)
	{
		logger->Info(Languages::TextStarting, name);

		if (senderConnection.IsConnected())
		{
			logger->Info(Languages::TextSenderSocketCreated);
		}
		else
		{
			logger->Error(Languages::TextUnableToCreateUdpSocketForSendingData);
		}
		receiverThread = std::thread(&Hardware::CS2::Receiver, this);
	}

	CS2::~CS2()
	{
		run = false;
		receiverConnection.Terminate();
		receiverThread.join();
		logger->Info(Languages::TextTerminatingSenderSocket);
	}

	void CS2::Booster(const boosterState_t status)
	{
		unsigned char buffer[CANCommandBufferLength];
		CreateBoosterCommand(buffer, status);
		if (senderConnection.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2::LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed)
	{
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoSpeedCommand(buffer, protocol, address, speed);
		if (senderConnection.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2::LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction)
	{
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoDirectionCommand(buffer, protocol, address, direction);
		if (senderConnection.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2::LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoFunctionCommand(buffer, protocol, address, function, on);
		if (senderConnection.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2::AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on)
	{
		unsigned char buffer[CANCommandBufferLength];
		CreateAccessoryCommand(buffer, protocol, address, state, on);
		if (senderConnection.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CS2::Receiver()
	{
		Utils::Utils::SetThreadName("CS2");
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
