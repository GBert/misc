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

#include "Hardware/IntelliboxBase.h"
#include "Languages.h"
#include "Utils/Utils.h"

namespace Hardware
{
	IntelliboxBase::IntelliboxBase(const HardwareParams* params, const std::string& controlName)
	:	SerialP50x(params, controlName)
	{
		SendP50XOnly();
		const bool ok = SendNop();
		if (!ok)
		{
			logger->Error(Languages::TextControlDoesNotAnswer);
			return;
		}

		s88Modules = Utils::Utils::StringToInteger(params->GetArg2(), 0);

		if (s88Modules > MaxS88ModulesIntellibox)
		{
			logger->Error(Languages::TextTooManyS88Modules, s88Modules, MaxS88ModulesIntellibox);
			return;
		}

		if (s88Modules == 0)
		{
			logger->Info(Languages::TextNoS88Modules);
			return;
		}

		logger->Info(Languages::TextNrOfS88Modules, s88Modules);
		unsigned char modules = SendXP88Get(0);
		if (modules != s88Modules)
		{
			SendXP88Set(0, s88Modules);
		}

		checkEventsThread = std::thread(&Hardware::ProtocolP50x::CheckEventsWorker, this);
	}
} // namespace
