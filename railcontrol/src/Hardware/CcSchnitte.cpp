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

#include "Hardware/CcSchnitte.h"
#include "Utils/Utils.h"

namespace Hardware
{
	extern "C" CcSchnitte* create_CcSchnitte(const HardwareParams* params)
	{
		return new CcSchnitte(params);
	}

	extern "C" void destroy_CcSchnitte(CcSchnitte* ccSchnitte)
	{
		delete(ccSchnitte);
	}

	CcSchnitte::CcSchnitte(const HardwareParams* params)
	:	MaerklinCAN(params->GetManager(),
			params->GetControlID(),
			Logger::Logger::GetLogger("CC-Schnitte " + params->GetName() + " " + params->GetArg1()),
			"CC-Schnitte / " + params->GetName() + " at serial port " + params->GetArg1()),
	 	serialLine(logger, params->GetArg1(), B500000, 8, 'N', 1),
		run(false)
	{
		logger->Info(Languages::TextStarting, name);

		if (!serialLine.IsConnected())
		{
			return;
		}

		receiverThread = std::thread(&Hardware::CcSchnitte::Receiver, this);
	}

	CcSchnitte::~CcSchnitte()
	{
		if (!run)
		{
			return;
		}
		run = false;
		receiverThread.join();
	}

	void CcSchnitte::Booster(const boosterState_t status)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateBoosterCommand(buffer, status);
		if (serialLine.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CcSchnitte::LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoSpeedCommand(buffer, protocol, address, speed);
		if (serialLine.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}


	void CcSchnitte::LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoDirectionCommand(buffer, protocol, address, direction);
		if (serialLine.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CcSchnitte::LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateLocoFunctionCommand(buffer, protocol, address, function, on);
		if (serialLine.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CcSchnitte::AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char buffer[CANCommandBufferLength];
		CreateAccessoryCommand(buffer, protocol, address, state, on);
		if (serialLine.Send(buffer, sizeof(buffer)) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}
	}

	void CcSchnitte::Receiver()
	{
		Utils::Utils::SetThreadName("CC-Schnitte");
		logger->Info(Languages::TextReceiverThreadStarted);
		run = true;
		while (run)
		{
			if (!serialLine.IsConnected())
			{
				logger->Error(Languages::TextUnableToReceiveData);
				return;
			}
			unsigned char buffer[CANCommandBufferLength];
			if (serialLine.ReceiveExact(buffer, sizeof(buffer)) != sizeof(buffer))
			{
				continue;
			}
			Parse(buffer);
		}
		logger->Info(Languages::TextTerminatingReceiverThread);
	}
} // namespace
