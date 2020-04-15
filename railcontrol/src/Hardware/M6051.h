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

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class M6051 : HardwareInterface
	{
		public:
			M6051(const HardwareParams* params);
			~M6051();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<protocol_t>& protocols) const override { protocols.push_back(ProtocolMM2); }

			bool LocoProtocolSupported(protocol_t protocol) const override { return (protocol == ProtocolMM2); }

			void GetAccessoryProtocols(std::vector<protocol_t>& protocols) const override { protocols.push_back(ProtocolMM2); }

			bool AccessoryProtocolSupported(protocol_t protocol) const override { return (protocol == ProtocolMM2); }

			static void GetArgumentTypesAndHint(std::map<unsigned char,argumentType_t>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = SerialPort;
				argumentTypes[2] = S88Modules;
				hint = Languages::GetText(Languages::TextHintM6051);
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;

		private:
			Logger::Logger* logger;
			static const unsigned char MaxS88Modules = 62;
			Network::Serial serialLine;
			volatile bool run;
			unsigned char s88Modules;
			std::thread s88Thread;
			unsigned char s88Memory[MaxS88Modules];
			std::map<address_t, unsigned char> speedMap;
			std::map<address_t, unsigned char> functionMap;

			unsigned char GetSpeedMapEntry(address_t address)
			{
				return speedMap.count(address) == 0 ? 0 : speedMap[address];
			}

			unsigned char GetFunctionMapEntry(address_t address)
			{
				return functionMap.count(address) == 0 ? 0 : functionMap[address];
			}

			void SendTwoBytes(const unsigned char byte1, const unsigned char byte2)
			{
				char dataArray[2];
				dataArray[0] = byte1;
				dataArray[1] = byte2;
				std::string data(dataArray, 2);
				serialLine.Send(data);
			}

			void S88Worker();
	};

	extern "C" M6051* create_M6051(const HardwareParams* params);
	extern "C" void destroy_M6051(M6051* m6051);

} // namespace

