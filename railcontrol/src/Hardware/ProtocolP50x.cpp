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

#include "Hardware/ProtocolP50x.h"
#include "Languages.h"
#include "Utils/Utils.h"

namespace Hardware
{
	ProtocolP50x::ProtocolP50x(const HardwareParams* const params,
		const std::string& controlName)
	:	HardwareInterface(params->GetManager(),
			params->GetControlID(),
			controlName,
			params->GetName()),
		run(false)
	{
		logger->Info(Languages::TextStarting, GetFullName());
	}

	ProtocolP50x::~ProtocolP50x()
	{
		if (!run)
		{
			return;
		}
		run = false;
		checkEventsThread.join();
	}

	void ProtocolP50x::Booster(const BoosterState status)
	{
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

	void ProtocolP50x::LocoSpeed(__attribute__((unused)) const Protocol protocol, const Address address, const Speed speed)
	{
		if (!CheckLocoAddress(address))
		{
			return;
		}

		cache.SetSpeed(address, speed);
		SendXLok(address);
	}


	void ProtocolP50x::LocoOrientation(__attribute__((unused)) const Protocol protocol, const Address address, const Orientation orientation)
	{
		if (!CheckLocoAddress(address))
		{
			return;
		}

		cache.SetOrientation(address, orientation);
		SendXLok(address);
	}

	void ProtocolP50x::LocoFunction(__attribute__((unused)) const Protocol protocol,
		const Address address,
		const DataModel::LocoFunctionNr function,
		const DataModel::LocoFunctionState on)
	{
		if (!CheckLocoAddress(address))
		{
			return;
		}

		if (function > MaxLocoFunctions)
		{
			return;
		}
		cache.SetFunction(address, function, on);
		std::lock_guard<std::mutex> guard(communicationLock);
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

	void ProtocolP50x::LocoSpeedOrientationFunctions(__attribute__((unused)) const Protocol protocol,
		const Address address,
		const Speed speed,
		const Orientation orientation,
		std::vector<DataModel::LocoFunctionEntry>& functions)
	{
		if (!CheckLocoAddress(address))
		{
			return;
		}

		cache.SetSpeed(address, speed);
		cache.SetOrientation(address, orientation);
		unsigned char nrFunctions = functions.size();
		for (const DataModel::LocoFunctionEntry& function : functions)
		{
			cache.SetFunction(address, function.nr, function.state);
		}
		std::lock_guard<std::mutex> guard(communicationLock);
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

	bool ProtocolP50x::SendXLok(const Address address) const
	{
		ProtocolP50xCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingSpeedOrientationLight, address, entry.speed, Languages::GetLeftRight(static_cast<Orientation>((entry.orientationF0 >> 5) & 0x01)), Languages::GetOnOff((entry.orientationF0 >> 4) & 0x01));
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[5] = { XLok, addressLSB, addressMSB, entry.speed, entry.orientationF0 };

		SendInternal(data, sizeof(data));
		unsigned char input;
		bool ret = ReceiveExactInternal(&input, 1);
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

	bool ProtocolP50x::SendXFunc(const Address address) const
	{
		ProtocolP50xCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions1_8, address, entry.function[0]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[4] = { XFunc, addressLSB, addressMSB, entry.function[0] };
		SendInternal(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool ProtocolP50x::SendXFunc2(const Address address) const
	{
		ProtocolP50xCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions9_16, address, entry.function[1]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[4] = { XFunc2, addressLSB, addressMSB, entry.function[1] };
		SendInternal(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool ProtocolP50x::SendXFunc34(const Address address) const
	{
		ProtocolP50xCacheEntry entry = cache.GetData(address);
		logger->Info(Languages::TextSettingFunctions17_28, address, entry.function[2], entry.function[3]);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char data[5] = { XFunc34, addressLSB, addressMSB, entry.function[2], entry.function[3] };
		SendInternal(data, sizeof(data));
		return ReceiveFunctionCommandAnswer();
	}

	bool ProtocolP50x::ReceiveFunctionCommandAnswer() const
	{
		unsigned char input;
		bool ret = ReceiveExactInternal(&input, 1);
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

	void ProtocolP50x::AccessoryOnOrOff(__attribute__((unused)) const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on)
	{
		if (!CheckAccessoryAddress(address))
		{
			return;
		}
		logger->Info(Languages::TextSettingAccessoryWithProtocol, ProtocolDCC, address, state, on);
		const unsigned char addressLSB = (address & 0xFF);
		const unsigned char addressMSB = (address >> 8);
		const unsigned char statusBits = ((state == DataModel::AccessoryStateOn) << 7) | (on << 6);
		const unsigned char addressStatus = addressMSB | statusBits;
		const unsigned char data[3] = { XTrnt, addressLSB, addressStatus };
		std::lock_guard<std::mutex> guard(communicationLock);
		SendInternal(data, sizeof(data));
		unsigned char input;
		bool ret = ReceiveExactInternal(&input, 1);
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

	bool ProtocolP50x::SendP50XOnly() const
	{
		unsigned char data[6] = { 'X', 'Z', 'z', 'A', '1', 0x0D };
		SendInternal(data, sizeof(data));
		const unsigned char dataLengthRead = 34;
		unsigned char input[dataLengthRead];
		ReceiveExactInternal(input, dataLengthRead);
		return true;
	}

	bool ProtocolP50x::SendOneByteCommand(const unsigned char data) const
	{
		std::lock_guard<std::mutex> guard(communicationLock);
		SendInternal(data);
		unsigned char input;
		int ret = ReceiveInternal(&input, 1);
		return ret > 0 && input == OK;
	}

	bool ProtocolP50x::SendRestart() const
	{
		unsigned char data[3] = { '@', '@', 0x0D };
		logger->Info(Languages::TextRestarting);
		std::lock_guard<std::mutex> guard(communicationLock);
		SendInternal(data, sizeof(data));
		return true;
	}

	unsigned char ProtocolP50x::SendXP88Get(unsigned char param) const
	{
		unsigned char data[2] = { XP88Get, param };
		std::lock_guard<std::mutex> guard(communicationLock);
		SendInternal(data, sizeof(data));
		unsigned char input;
		size_t ret = ReceiveExactInternal(&input, 1);
		if (ret == 0 || input != OK)
		{
			return 0xFF;
		}
		ret = ReceiveExactInternal(&input, 1);
		if (ret == 0)
		{
			return 0xFF;
		}
		return input;
	}

	bool ProtocolP50x::SendXP88Set(unsigned char param, unsigned char value) const
	{
		unsigned char data[3] = { XP88Set, param, value };
		std::lock_guard<std::mutex> guard(communicationLock);
		SendInternal(data, sizeof(data));
		unsigned char input;
		size_t ret = ReceiveExactInternal(&input, 1);
		if (ret == 0)
		{
			return false;
		}
		return (input == OK);
	}

	void ProtocolP50x::CheckSensorData(const unsigned char module, const unsigned char data) const
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

	void ProtocolP50x::SendXEvtSen() const
	{
		std::lock_guard<std::mutex> guard(communicationLock);
		unsigned char data[1] = { XEvtSen };
		SendInternal(data, sizeof(data));
		while (true)
		{
			unsigned char module;
			size_t ret = ReceiveExactInternal(&module, 1);
			if (ret == 0 || module == 0)
			{
				return;
			}

			--module;
			module <<= 1;

			unsigned char data[2];
			ret = ReceiveExactInternal(data, sizeof(data));
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

	void ProtocolP50x::SendXEvent() const
	{
		unsigned char input;
		{
			std::lock_guard<std::mutex> guard(communicationLock);
			unsigned char data[1] = { XEvent };
			SendInternal(data, sizeof(data));
			size_t ret = ReceiveExactInternal(&input, 1);
			if (ret == 0)
			{
				return;
			}
			while (true)
			{
				bool moreData = (input >> 7) & 0x01;
				if (!moreData)
				{
					break;
				}

				size_t ret = ReceiveExactInternal(&input, 1);
				if (ret == 0)
				{
					break;
				}
			}
		}

		bool sensorEvent = (input >> 2) & 0x01;
		if (sensorEvent)
		{
			SendXEvtSen();
		}

		bool locoEvent = input & 0x01;
		if (locoEvent)
		{
			logger->Debug(Languages::TextLocoEventDetected);
		}

		bool powerEvent = (input >> 3) & 0x01;
		if (powerEvent)
		{
			manager->Booster(ControlTypeHardware, BoosterStateStop);
		}

		bool switchEvent = (input >> 5) & 0x01;
		if (switchEvent)
		{
			logger->Debug(Languages::TextSwitchEventDetected);
		}
	}

	void ProtocolP50x::CheckEventsWorker()
	{
		Utils::Utils::SetThreadName(GetShortName());
		run = true;
		while (run)
		{
			SendXEvent();
			std::this_thread::yield();
		}
	}
} // namespace
