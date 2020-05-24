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
#include <sstream>
#include <string>

#include "Network/Select.h"
#include "Hardware/M6051.h"
#include "Utils/Utils.h"

using std::string;

namespace Hardware
{

	// create instance of m6051
	extern "C" M6051* create_M6051(const HardwareParams* params)
	{
		return new M6051(params);
	}

	// delete instance of m6051
	extern "C" void destroy_M6051(M6051* m6051)
	{
		delete(m6051);
	}

	M6051::M6051(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(), params->GetControlID(), "Maerklin Interface (6050/6051) / " + params->GetName() + " at serial port " + params->GetArg1()),
	 	logger(Logger::Logger::GetLogger("M6051 " + params->GetName() + " " + params->GetArg1())),
	 	serialLine(logger, params->GetArg1(), B2400, 8, 'N', 2),
		run(true)
	{
		logger->Info(Languages::TextStarting, name);

		s88Modules = Utils::Utils::StringToInteger(params->GetArg2(), 0, MaxS88Modules);
		if (s88Modules == 0)
		{
			logger->Info(Languages::TextNoS88Modules);
			return;
		}
		logger->Info(Languages::TextNrOfS88Modules, s88Modules);
		s88Thread = std::thread(&Hardware::M6051::S88Worker, this);
	}

	M6051::~M6051()
	{
		run = false;
		if (s88Modules > 0)
		{
			s88Thread.join();
		}
	}

	void M6051::Booster(const BoosterState status)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}

		unsigned char c;

		if (status)
		{
			logger->Info(Languages::TextTurningBoosterOn);
			c = 96;
		}
		else
		{
			logger->Info(Languages::TextTurningBoosterOn);
			c = 97;
		}
		serialLine.Send(c);
	}

	void M6051::LocoSpeed(__attribute__((unused)) const Protocol protocol, const Address address, const Speed speed)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		unsigned char speedMM = (speed / 69) + (GetSpeedMapEntry(address) & 16);
		speedMap[address] = speedMM;
		unsigned char addressMM = static_cast<unsigned char>(address);
		logger->Info(Languages::TextSettingSpeed, address, speedMM);
		SendTwoBytes(speedMM, addressMM);
	}

	void M6051::LocoDirection(__attribute__((unused)) const Protocol protocol, const Address address, __attribute__((unused)) const Direction direction)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}
		logger->Info(Languages::TextSettingDirection, address);
		unsigned char speedMM = 15 + (GetSpeedMapEntry(address) & 16);
		unsigned char addressMM = static_cast<unsigned char>(address);
		SendTwoBytes(speedMM, addressMM);
	}

	void M6051::LocoFunction(__attribute__((unused)) const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on)
	{
		if (function > 4)
		{
			return;
		}

		if (!serialLine.IsConnected())
		{
			return;
		}

		logger->Info(Languages::TextSettingFunction, static_cast<int>(function), address, Languages::GetOnOff(on));
		unsigned char addressMM = static_cast<unsigned char>(address);
		if (function == 0)
		{
			unsigned char speedMM = (GetSpeedMapEntry(address) & 15) + (static_cast<unsigned char>(on) << 4);
			speedMap[address] = speedMM;
			SendTwoBytes(speedMM, addressMM);
			return;
		}

		unsigned char functionMM = GetFunctionMapEntry(address);
		unsigned char position = function - 1;
		functionMM &= (~(1 << position)); // mask out related function
		functionMM |= (static_cast<unsigned char>(on == DataModel::LocoFunctions::FunctionStateOn) << position); // add related function
		functionMap[address] = functionMM;
		functionMM += 64;
		SendTwoBytes(functionMM, addressMM);
	}

	void M6051::AccessoryOnOrOff(__attribute__((unused)) const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}

		logger->Info(Languages::TextSettingAccessory, address, Languages::GetGreenRed(state), Languages::GetOnOff(on));
		const unsigned char stateMM = (state == DataModel::AccessoryStateOn ? 33 : 34);
		const unsigned char addressMM = static_cast<unsigned char>(address);
		SendTwoBytes(stateMM, addressMM);
	}

	void M6051::S88Worker()
	{
		Utils::Utils::SetThreadName("M6051");
		const unsigned char s88DoubleModules = ((s88Modules + 1) / 2);
		const unsigned char command = 128 + s88DoubleModules;
		const unsigned char s88SingleModules = (s88DoubleModules * 2);
		while(run && serialLine.IsConnected())
		{
			serialLine.ClearBuffers();
			serialLine.Send(command);
			for (unsigned char module = 0; module < s88SingleModules; ++module)
			{
				string data;
				bool ret = serialLine.Receive(data, 1);
				if (ret <= 0)
				{
					logger->Error(Languages::TextUnableToReceiveData);
					break;
				}
				unsigned char byte = data[0];

				if (byte != s88Memory[module])
				{
					unsigned char xorByte = byte ^ s88Memory[module];
					for(unsigned char pin = 1; pin <= 8; ++pin)
					{
						unsigned char shift = 8 - pin;
						if (((xorByte >> shift) & 0x01) == 0)
						{
							continue;
						}

						const char* onOff;
						DataModel::Feedback::FeedbackState state;
						if ((byte >> shift) & 0x01)
						{
							onOff = Languages::GetText(Languages::TextOn);
							state = DataModel::Feedback::FeedbackStateOccupied;
						}
						else
						{
							onOff = Languages::GetText(Languages::TextOff);
							state = DataModel::Feedback::FeedbackStateFree;
						}
						logger->Info(Languages::TextFeedbackChange, pin, module, onOff);
						Address address = (module * 8) + pin;
						manager->FeedbackState(controlID, address, state);
					}
					s88Memory[module] = byte;
				}
			}
			std::this_thread::yield();
		}
	}
} // namespace
