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

#include "Hardware/DccPpExSerial.h"
#include "Utils/Utils.h"

namespace Hardware
{
	DccPpExSerial::DccPpExSerial(const HardwareParams* params)
	:	DccPpEx(params,
			"Dcc++EX Serial / " + params->GetName() + " at serial port " + params->GetArg1(),
			params->GetName()),
	 	serialLine(logger, params->GetArg1(), B115200, 8, 'N', 1, true)
	{
		logger->Info(Languages::TextStarting, GetFullName());
	}

	void DccPpExSerial::Send(const std::string& buffer)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		if (serialLine.Send(buffer) == -1)
		{
			logger->Error(Languages::TextUnableToSendDataToControl);
		}

		std::string dummy;
		serialLine.Receive(dummy);
	}
} // namespace
