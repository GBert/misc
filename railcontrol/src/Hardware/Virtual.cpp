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

#include <future>
#include <sstream>
#include <string>

#include "Hardware/Virtual.h"
#include "Manager.h"
#include "Utils/Utils.h"

namespace Hardware
{
	Virtual::Virtual(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(),
			params->GetControlID(),
			"Virtual Command Station / " + params->GetName(),
			params->GetName())
	{
	}

	// turn booster on or off
	void Virtual::Booster(const BoosterState status)
	{
		logger->Info(status ? Languages::TextTurningBoosterOn : Languages::TextTurningBoosterOff);
	}

	// set loco speed
	void Virtual::LocoSpeed(const Protocol protocol, const Address address, const Speed speed)
	{
		logger->Info(Languages::TextSettingSpeedWithProtocol, protocol, address, speed);
	}

	// set the direction of a loco
	void Virtual::LocoOrientation(const Protocol protocol, const Address address, const Orientation orientation)
	{
		logger->Info(Languages::TextSettingDirectionOfTravelWithProtocol, protocol, address, Languages::GetLeftRight(orientation));
	}

	// set loco function
	void Virtual::LocoFunction(const Protocol protocol,
		const Address address,
		const DataModel::LocoFunctionNr function,
		const DataModel::LocoFunctionState on)
	{
		logger->Info(Languages::TextSettingFunctionWithProtocol, static_cast<int>(function), static_cast<int>(protocol), address, Languages::GetOnOff(on));
	}

	// accessory command
	void Virtual::AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on)
	{
		logger->Info(Languages::TextSettingAccessoryWithProtocol, static_cast<int>(protocol), address, Languages::GetGreenRed(state), Languages::GetOnOff(on));
	}

	// read CV value
	void Virtual::ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv)
	{
		switch (mode)
		{
			case ProgramModeMfx:
				logger->Info(Languages::TextProgramMfxRead, address, cv);
				break;

			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccRead, cv);
				break;

			case ProgramModeDccPomLoco:
				logger->Info(Languages::TextProgramDccPomLocoRead, address, cv);
				break;

			case ProgramModeDccPomAccessory:
				logger->Info(Languages::TextProgramDccPomAccessoryRead, address, cv);
				break;

			default:
				return;
		}
		std::async(std::launch::async, Manager::ProgramDccValueStatic, manager, cv, cv & 0xFF);
	}

	// write DCC CV value
	void Virtual::ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value)
	{
		switch (mode)
		{
			case ProgramModeMm:
				logger->Info(Languages::TextProgramMm, cv, value);
				break;

			case ProgramModeMmPom:
				logger->Info(Languages::TextProgramMmPom, address, cv, value);
				break;

			case ProgramModeMfx:
				logger->Info(Languages::TextProgramMfxWrite, address, cv, value);
				break;

			case ProgramModeDccDirect:
				logger->Info(Languages::TextProgramDccWrite, cv, value);
				break;

			case ProgramModeDccPomLoco:
				logger->Info(Languages::TextProgramDccPomLocoWrite, address, cv, value);
				break;

			case ProgramModeDccPomAccessory:
				logger->Info(Languages::TextProgramDccPomAccessoryWrite, address, cv, value);
				break;

			default:
				return;
		}
	}
} // namespace
