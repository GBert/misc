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

#include <cstring>    //memset
#include <fcntl.h>
#include <sstream>
#include <termios.h>

#include "Languages.h"
#include "Network/Select.h"
#include "Hardware/Hsi88.h"
#include "Utils/Utils.h"

namespace Hardware
{

	// create instance of Hsi88
	extern "C" Hsi88* create_Hsi88(const HardwareParams* params)
	{
		return new Hsi88(params);
	}

	// delete instance of Hsi88
	extern "C" void destroy_Hsi88(Hsi88* hsi88)
	{
		delete(hsi88);
	}

	Hsi88::Hsi88(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(), params->GetControlID(), "HSI-88 / " + params->GetName() + " at serial port " + params->GetArg1()),
	 	logger(Logger::Logger::GetLogger("HSI-88 " + params->GetName() + " " + params->GetArg1())),
	 	serialLine(logger, params->GetArg1(), B9600, 8, 'N', 1),
		run(false)
	{
		logger->Info(Languages::TextStarting, name);

		memset(s88Init, 0xFF, sizeof(s88Memory));
		memset(s88Memory, 0x00, sizeof(s88Memory));

		s88Modules1 = Utils::Utils::StringToInteger(params->GetArg2(), 0);
		s88Modules2 = Utils::Utils::StringToInteger(params->GetArg3(), 0);
		s88Modules3 = Utils::Utils::StringToInteger(params->GetArg4(), 0);
		s88Modules = s88Modules1 + s88Modules2 + s88Modules3;

		if (s88Modules > MaxS88Modules)
		{
			logger->Error(Languages::TextTooManyS88Modules, s88Modules, MaxS88Modules);
			return;
		}

		if (s88Modules == 0)
		{
			logger->Error(Languages::TextNoS88Modules);
			return;
		}

		std::string version = GetVersion();
		logger->Info(Languages::TextVersion, version);

		unsigned char modulesConfigured = ConfigureS88();
		if (s88Modules != modulesConfigured)
		{
			logger->Error(Languages::TextHsi88ErrorConfiguring, modulesConfigured);
			return;
		}

		logger->Info(Languages::TextHsi88Configured, s88Modules, s88Modules1, s88Modules2, s88Modules3);

		checkEventsThread = std::thread(&Hardware::Hsi88::CheckEventsWorker, this);
	}

	Hsi88::~Hsi88()
	{
		if (!run)
		{
			return;
		}
		run = false;
		checkEventsThread.join();
	}

	std::string Hsi88::ReadUntilCR()
	{
		std::string data;
		unsigned char input = 0;
		while (true)
		{
			int ret = serialLine.Receive(&input, sizeof(input), 1, 0);
			if (ret < 0 || input == '\r')
			{
				logger->Hex(data);
				return data;
			}
			data.append(reinterpret_cast<char*>(&input), ret);
		}
	}

	std::string Hsi88::GetVersion()
	{
		const unsigned char command[2] = { 'v', '\r' };
		logger->Hex(command, sizeof(command));
		serialLine.Send(command, sizeof(command));
		return ReadUntilCR();
	}

	unsigned char Hsi88::ConfigureS88()
	{
		const unsigned char command [5] = { 's', static_cast<unsigned char>(s88Modules1 >> 1), static_cast<unsigned char>(s88Modules2 >> 1), static_cast<unsigned char>(s88Modules3 >> 1), '\r' };
		serialLine.Send(command, sizeof(command));
		logger->Hex(command, sizeof(command));
		unsigned char input[3];
		int ret = serialLine.ReceiveExact(input, sizeof(input));
		logger->Hex(input, sizeof(input));
		if (ret <= 0)
		{
			return 0;
		}
		if (input[0] != 's')
		{
			return 0;
		}
		return input[1] << 1;
	}

	void Hsi88::ReadData()
	{
		std::string data;
		const unsigned char headerDataSize = 3;
		serialLine.ReceiveExact(data, headerDataSize);
		if (data.size() != headerDataSize)
		{
			return;
		}
		if (data[0] != 'i')
		{
			return;
		}
		const unsigned char modules = data[1];
		const unsigned char moduleDataSize = modules * 3;
		const unsigned char commandSize = moduleDataSize + headerDataSize;
		serialLine.ReceiveExact(data, moduleDataSize);
		logger->Hex(data);
		if (data.size() != commandSize)
		{
			return;
		}
		for (unsigned char module = 0; module < modules; ++module)
		{
			const unsigned char modulePosition = module * 3 + 2;
			const unsigned char* moduleData = reinterpret_cast<const unsigned char*>(data.c_str()) + modulePosition;
			if (*moduleData > (MaxS88Modules >> 1))
			{
				continue;
			}
			const unsigned char memoryPosition = (data[modulePosition] - 1) * 2;
			CheckFeedbackByte(moduleData[2], memoryPosition);
			CheckFeedbackByte(moduleData[1], memoryPosition + 1);
		}
	}

	void Hsi88::CheckFeedbackByte(const unsigned char dataByte, const unsigned char module)
	{
		unsigned char diff = (s88Memory[module] ^ dataByte) | s88Init[module];
		if (!diff)
		{
			return;
		}
		unsigned char pin = 0;
		while (diff)
		{
			if (diff & 0x01)
			{
				DataModel::Feedback::FeedbackState state = static_cast<DataModel::Feedback::FeedbackState>((dataByte >> pin) & 0x01);
				logger->Info(Languages::TextFeedbackChange, pin + 1, module, Languages::GetOnOff(state));
				manager->FeedbackState(controlID, module * 8 + pin + 1, state);
			}
			diff >>= 1;
			++pin;
		}
		s88Memory[module] = dataByte;
		s88Init[module] = 0;
	}

	void Hsi88::CheckEventsWorker()
	{
		Utils::Utils::SetThreadName("HSI-88");
		run = true;
		while (run)
		{
			ReadData();
			std::this_thread::yield();
		}
	}
} // namespace
