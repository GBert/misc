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

#include "Hardware/OpenDcc.h"
#include "Languages.h"
#include "Utils/Utils.h"

namespace Hardware
{
	OpenDcc::OpenDcc(const HardwareParams* params)
	:	ProtocolP50xSerial(params, "OpenDCC")
	{
		SendP50XOnly();
		bool ok = SendNop();
		if (!ok)
		{
			logger->Error(Languages::TextControlDoesNotAnswer);
			return;
		}

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
			logger->Info(Languages::TextNoS88Modules);
			return;
		}

		logger->Info(Languages::TextHsi88Configured, s88Modules, s88Modules1, s88Modules2, s88Modules3);
		bool restart = false;
		unsigned char modules = SendXP88Get(0);
		if (modules != s88Modules)
		{
			logger->Info(Languages::TextNrOfS88Modules, s88Modules);
			SendXP88Set(0, s88Modules);
			restart = true;;
		}

		modules = SendXP88Get(1);
		if (modules != s88Modules1)
		{
			logger->Info(Languages::TextNrOfS88ModulesOnBus, s88Modules1, 1);
			SendXP88Set(1, s88Modules1);
			restart = true;;
		}

		modules = SendXP88Get(2);
		if (modules != s88Modules2)
		{
			logger->Info(Languages::TextNrOfS88ModulesOnBus, s88Modules2, 2);
			SendXP88Set(2, s88Modules2);
			restart = true;;
		}

		modules = SendXP88Get(3);
		if (modules != s88Modules3)
		{
			logger->Info(Languages::TextNrOfS88ModulesOnBus, s88Modules3, 3);
			SendXP88Set(3, s88Modules3);
			restart = true;;
		}

		if (restart)
		{
			SendXP88Set(4, 0);
			SendRestart();
			Utils::Utils::SleepForMilliseconds(100);
			SendP50XOnly();
			ok = SendNop();
			if (!ok)
			{
				logger->Error(Languages::TextControlDoesNotAnswer);
				return;
			}
		}

		checkEventsThread = std::thread(&Hardware::ProtocolP50x::CheckEventsWorker, this);
	}
} // namespace
