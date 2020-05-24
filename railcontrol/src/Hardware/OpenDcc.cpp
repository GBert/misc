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

#include "Hardware/OpenDcc.h"
#include "Utils/Utils.h"

namespace Hardware
{

	// create instance of OpenDcc
	extern "C" OpenDcc* create_OpenDcc(const HardwareParams* params)
	{
		return new OpenDcc(params);
	}

	// delete instance of OpenDcc
	extern "C" void destroy_OpenDcc(OpenDcc* opendcc)
	{
		delete(opendcc);
	}

	OpenDcc::OpenDcc(const HardwareParams* params)
	:	HardwareInterface(params->GetManager(), params->GetControlID(), "OpenDCC / " + params->GetName() + " at serial port " + params->GetArg1()),
	 	logger(Logger::Logger::GetLogger("OpenDCC " + params->GetName() + " " + params->GetArg1())),
	 	serialLine(logger, params->GetArg1(), B19200, 8, 'N', 2),
		run(false)
	{
		logger->Info(Languages::TextStarting, name);

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

		checkEventsThread = std::thread(&Hardware::OpenDcc::CheckEventsWorker, this);
	}

	OpenDcc::~OpenDcc()
	{
		if (!run)
		{
			return;
		}
		run = false;
		checkEventsThread.join();
	}

	void OpenDcc::Booster(const BoosterState status)
	{
		if (!serialLine.IsConnected())
		{
			return;
		}

		if (status)
		{
			logger->Info(Languages::TextTurningBoosterOn);
			SendPowerOn();
		}
		else
		{
			logger->Info(Languages::TextTurningBoosterOff);
			SendPowerOff();
		}
	}

	void OpenDcc::LocoSpeed(__attribute__((unused)) const Protocol protocol, const Address address, const Speed speed)
	{
		if (!serialLine.IsConnected() || !CheckLocoAddress(address))
		{
			return;
		}

		cache.SetSpeed(address, speed);
		SendXLok(address);
	}


	void OpenDcc::LocoDirection(__attribute__((unused)) const Protocol protocol, const Address address, const Direction direction)
	{
		if (!serialLine.IsConnected() || !CheckLocoAddress(address))
		{
			return;
		}

		cache.SetDirection(address, direction);
		SendXLok(address);
	}

	void OpenDcc::LocoFunction(__attribute__((unused)) const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on)
	{
		if (!serialLine.IsConnected() || !CheckLocoAddress(address))
		{
			return;
		}

		if (function > MaxLocoFunctions)
		{
			return;
		}
		cache.SetFunction(address, function, on);
		if (function == 0)
		{
			SendXLok(address);
			return;
		}

		if (function <= 8)
		{
			SendXFunc(address);
			return;
		}

		if (function <= 16)
		{
			SendXFunc2(address);
			return;
		}

		SendXFunc34(address);
	}

	void OpenDcc::LocoSpeedDirectionFunctions(__attribute__((unused)) const Protocol protocol, const Address address, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions)
	{
		if (!serialLine.IsConnected() || !CheckLocoAddress(address))
		{
			return;
		}

		cache.SetSpeed(address, speed);
		cache.SetDirection(address, direction);
		unsigned char nrFunctions = functions.size();
		for (unsigned char functionNr = 0; functionNr < nrFunctions; ++functionNr)
		{
			cache.SetFunction(address, functionNr, functions[functionNr]);
		}
		SendXLok(address);
		if (nrFunctions > 1)
		{
			SendXFunc(address);
			if (nrFunctions > 8)
			{
				SendXFunc2(address);
				if (nrFunctions > 16)
				{
					SendXFunc34(address);
				}
			}
		}
	}

	bool OpenDcc::SendXLok(const Address address) const
	{
		OpenDccCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingSpeedDirectionLight, address, entry.speed, Languages::GetLeftRight(static_cast<Direction>((entry.directionF0 >> 5) & 0x01)), Languages::GetOnOff((entry.directionF0 >> 4) & 0x01));
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[5] = { XLok, addressLSB, addressMSB, entry.speed, entry.directionF0 };

		serialLine.Send(data, sizeof(data));
		unsigned char input;
		bool ret = serialLine.ReceiveExact(&input, 1);
		if (ret != 1)
		{
			logger->Warning(Languages::TextControlDoesNotAnswer);
			return false;
		}
		switch (input)
		{
			case OK:
				return true;

			case XBADPRM:
				logger->Warning(Languages::TextControlReturnedBadParameter);
				return false;

			case XLKHALT:
				logger->Warning(Languages::TextControlReturnedOnHalt);
				return false;

			case XLKPOFF:
				logger->Warning(Languages::TextControlReturnedPowerOff);
				return false;

			default:
				logger->Warning(Languages::TextControlReturnedUnknownErrorCode, static_cast<int>(input));
				return false;
		}
	}

	bool OpenDcc::SendXFunc(const Address address) const
	{
		OpenDccCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions1_8, address, entry.function[0]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[4] = { XFunc, addressLSB, addressMSB, entry.function[0] };
		serialLine.Send(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool OpenDcc::SendXFunc2(const Address address) const
	{
		OpenDccCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions9_16, address, entry.function[1]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[4] = { XFunc2, addressLSB, addressMSB, entry.function[1] };
		serialLine.Send(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool OpenDcc::SendXFunc34(const Address address) const
	{
		OpenDccCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions17_28, address, entry.function[2], entry.function[3]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[5] = { XFunc34, addressLSB, addressMSB, entry.function[2], entry.function[3] };
		serialLine.Send(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool OpenDcc::ReceiveFunctionCommandAnswer() const
	{
		unsigned char input;
		bool ret = serialLine.ReceiveExact(&input, 1);
		if (ret != 1)
		{
			logger->Warning(Languages::TextControlDoesNotAnswer);
			return false;
		}
		switch (input)
		{
			case OK:
				return true;

			case XBADPRM:
				logger->Warning(Languages::TextControlReturnedBadParameter);
				return false;

			case XNOSLOT:
				logger->Warning(Languages::TextControlReturnedQueueFull);
				return false;

			default:
				logger->Warning(Languages::TextControlReturnedUnknownErrorCode, static_cast<int>(input));
				return false;
		}
	}

	void OpenDcc::AccessoryOnOrOff(__attribute__((unused)) const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on)
	{
		if (!serialLine.IsConnected() || !CheckAccessoryAddress(address))
		{
			return;
		}
		logger->Info(Languages::TextSettingAccessoryWithProtocol, ProtocolDCC, address, state, on);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char statusBits = ((state == DataModel::AccessoryStateOn) << 7) | (on << 6);
		const unsigned char addressStatus = addressMSB | statusBits;
		const unsigned char data[3] = { XTrnt, addressLSB, addressStatus };
		serialLine.Send(data, sizeof(data));
		unsigned char input;
		bool ret = serialLine.ReceiveExact(&input, 1);
		if (ret != 1)
		{
			logger->Warning(Languages::TextControlDoesNotAnswer);
			return;
		}
		switch (input)
		{
			case OK:
				return;

			case XBADPRM:
				logger->Warning(Languages::TextControlReturnedBadParameter);
				return;

			case XLOWTSP:
				logger->Warning(Languages::TextControlReturnedQueueNearlyFull);
				return;

			default:
				logger->Warning(Languages::TextControlReturnedUnknownErrorCode, static_cast<int>(input));
				return;
		}
	}

	bool OpenDcc::SendP50XOnly() const
	{
		unsigned char data[6] = { 'X', 'Z', 'Z', 'A', '1', 0x0D };
		serialLine.Send(data, sizeof(data));
		std::string input;
		serialLine.ReceiveExact(input, 34);
		return true;
	}

	bool OpenDcc::SendOneByteCommand(const unsigned char data) const
	{
		serialLine.Send(data);
		unsigned char input[1];
		int ret = serialLine.Receive(input, sizeof(input));
		return ret > 0 && input[0] == OK;
	}

	bool OpenDcc::SendRestart() const
	{
		unsigned char data[3] = { '@', '@', 0x0D };
		logger->Info(Languages::TextRestarting);
		serialLine.Send(data, sizeof(data));
		return true;
	}

	unsigned char OpenDcc::SendXP88Get(unsigned char param) const
	{
		unsigned char data[2] = { XP88Get, param };
		serialLine.Send(data, sizeof(data));
		unsigned char input;
		size_t ret = serialLine.ReceiveExact(&input, 1);
		if (ret == 0 || input != OK)
		{
			return 0xFF;
		}
		ret = serialLine.ReceiveExact(&input, 1);
		if (ret == 0)
		{
			return 0xFF;
		}
		return input;
	}

	bool OpenDcc::SendXP88Set(unsigned char param, unsigned char value) const
	{
		unsigned char data[3] = { XP88Set, param, value };
		serialLine.Send(data, sizeof(data));
		unsigned char input;
		size_t ret = serialLine.ReceiveExact(&input, 1);
		if (ret == 0)
		{
			return false;
		}
		return (input == OK);
	}

	void OpenDcc::CheckSensorData(const unsigned char module, const unsigned char data) const
	{
		unsigned char diff = s88Memory[module] ^ data;
		s88Memory[module] = data;
		FeedbackPin pinOverAll = module;
		pinOverAll <<= 3;
		for (unsigned char pinOnModule = 1; pinOnModule <= 8; ++pinOnModule)
		{
			if ((diff >> (8 - pinOnModule)) & 0x01)
			{
				DataModel::Feedback::FeedbackState state = static_cast<DataModel::Feedback::FeedbackState>((data >> (8 - pinOnModule)) & 0x01);
				logger->Info(Languages::TextFeedbackChange, pinOnModule, module, Languages::GetText(state ? Languages::TextOn : Languages::TextOff));
				manager->FeedbackState(controlID, pinOverAll + pinOnModule, state);
			}
		}
	}

	void OpenDcc::SendXEvtSen() const
	{
		unsigned char data[1] = { XEvtSen };
		serialLine.Send(data, sizeof(data));
		while (true)
		{
			unsigned char module;
			size_t ret = serialLine.ReceiveExact(&module, 1);
			if (ret == 0 || module == 0)
			{
				return;
			}

			--module;
			module <<= 1;

			unsigned char data[2];
			ret = serialLine.ReceiveExact(data, sizeof(data));
			if (ret == 0)
			{
				return;
			}

			if (s88Memory[module] != data[0])
			{
				CheckSensorData(module, data[0]);
			}

			++module;

			if (s88Memory[module] != data[1])
			{
				CheckSensorData(module, data[1]);
			}
		}
	}

	void OpenDcc::SendXEvent() const
	{
		unsigned char data[1] = { XEvent };
		serialLine.Send(data, sizeof(data));
		unsigned char input;
		size_t ret = serialLine.ReceiveExact(&input, 1);
		if (ret == 0)
		{
			return;
		}
		bool locoEvent = input & 0x01;
		bool sensorEvent = (input >> 2) & 0x01;
		bool powerEvent = (input >> 3) & 0x01;
		bool switchEvent = (input >> 5) & 0x01;

		while (true)
		{
			bool moreData = (input >> 7) & 0x01;
			if (!moreData)
			{
				break;
			}
			ret = serialLine.ReceiveExact(&input, 1);
			if (ret == 0)
			{
				break;
			}
		}

		if (sensorEvent)
		{
			SendXEvtSen();
		}

		if (locoEvent)
		{
			logger->Debug(Languages::TextLocoEventDetected);
		}

		if (powerEvent)
		{
			manager->Booster(ControlTypeHardware, BoosterStateStop);
		}

		if (switchEvent)
		{
			logger->Debug(Languages::TextSwitchEventDetected);
		}
	}

	void OpenDcc::CheckEventsWorker()
	{
		Utils::Utils::SetThreadName("OpenDcc");
		run = true;
		while (run)
		{
			SendXEvent();
			std::this_thread::yield();
		}
	}
} // namespace
