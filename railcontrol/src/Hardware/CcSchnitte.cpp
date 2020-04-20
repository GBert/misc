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
	:	ProtocolMaerklinCAN(params->GetManager(),
			params->GetControlID(),
			Logger::Logger::GetLogger("CC-Schnitte " + params->GetName() + " " + params->GetArg1()),
			"CC-Schnitte / " + params->GetName() + " at serial port " + params->GetArg1()),
	 	serialLine(logger, params->GetArg1(), B500000, 8, 'N', 1, true),
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
		if (run == false)
		{
			return;
		}
		run = false;
		receiverThread.join();
	}

	void CcSchnitte::Send(const unsigned char* buffer)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		logger->Hex(buffer, CANCommandBufferLength);
		if (serialLine.Send(buffer, CANCommandBufferLength) == -1)
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
			ssize_t datalen = serialLine.ReceiveExact(buffer, sizeof(buffer));
			if (run == false)
			{
				break;
			}
			if (datalen == 0)
			{
				continue;
			}
			if (datalen != sizeof(buffer))
			{
				logger->Error(Languages::TextInvalidDataReceived);
				continue;
			}
			logger->Hex(buffer, sizeof(buffer));
			Parse(buffer);
		}
		logger->Info(Languages::TextTerminatingReceiverThread);
	}
} // namespace
