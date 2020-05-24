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

#pragma once

#include <string>

#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Hardware/OpenDccCache.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class OpenDcc : HardwareInterface
	{
		public:
			OpenDcc(const HardwareParams* params);
			~OpenDcc();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolDCC); }

			bool LocoProtocolSupported(Protocol protocol) const override { return (protocol == ProtocolDCC); }

			void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolDCC); }

			bool AccessoryProtocolSupported(Protocol protocol) const override { return (protocol == ProtocolDCC); }

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				argumentTypes[2] = ArgumentTypeS88Modules;
				argumentTypes[3] = ArgumentTypeS88Modules;
				argumentTypes[4] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintOpenDcc);
			}

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoDirection(const Protocol protocol, const Address address, const Direction direction) override;
			void LocoFunction(const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void LocoSpeedDirectionFunctions(const Protocol protocol, const Address address, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions) override;
			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;

		private:
			enum Commands : unsigned char
			{
				XNop = 0xC4,
				XPwrOn = 0xA7,
				XPwrOff = 0xA6,
				XLok = 0x80,
				XFunc = 0x88,
				XFunc2 = 0x89,
				XFunc34 = 0x8A,
				XTrnt = 0x90,
				XP88Get = 0x9C,
				XP88Set = 0x9D,
				XEvent = 0xC8,
				XEvtLok = 0xC9,
				XEvtTrnt = 0xCA,
				XEvtSen = 0xCB
			};
			enum Answers : unsigned char
			{
				OK = 0x00,
				XBADPRM = 0x02,
				XPWOFF = 0x06,
				XNODATA = 0x0A,
				XNOSLOT = 0x0B,
				XLOWTSP = 0x40,
				XLKHALT = 0x41,
				XLKPOFF = 0x42
			};
			static const unsigned char MaxS88Modules = 128;
			static const unsigned char MaxLocoFunctions = 28;
			static const unsigned short MaxLocoAddress = 10239;
			static const unsigned short MaxAccessoryAddress = 2043;

			Logger::Logger* logger;
			mutable Network::Serial serialLine;
			volatile bool run;
			unsigned char s88Modules1;
			unsigned char s88Modules2;
			unsigned char s88Modules3;
			unsigned short s88Modules;

			std::thread checkEventsThread;
			mutable unsigned char s88Memory[MaxS88Modules];

			Hardware::OpenDccCache cache;

			static bool CheckLocoAddress(const Address address) { return 0 < address && address <= MaxLocoAddress; }
			static bool CheckAccessoryAddress(const Address address) { return 0 < address && address <= MaxAccessoryAddress; }

			bool SendP50XOnly() const;
			bool SendOneByteCommand(const unsigned char data) const;
			bool SendNop() const { return SendOneByteCommand(XNop); }
			bool SendPowerOn() const { return SendOneByteCommand(XPwrOn); }
			bool SendPowerOff() const { return SendOneByteCommand(XPwrOff); }
			bool SendXLok(const Address address) const;
			bool SendXFunc(const Address address) const;
			bool SendXFunc2(const Address address) const;
			bool SendXFunc34(const Address address) const;
			bool ReceiveFunctionCommandAnswer() const;
			bool SendRestart() const;
			unsigned char SendXP88Get(unsigned char param) const;
			bool SendXP88Set(unsigned char param, unsigned char value) const;
			void CheckSensorData(const unsigned char module, const unsigned char data) const;
			void SendXEvtSen() const;
			void SendXEvent() const;

			void CheckEventsWorker();
	};

	extern "C" OpenDcc* create_OpenDcc(const HardwareParams* params);
	extern "C" void destroy_OpenDcc(OpenDcc* opendcc);

} // namespace

