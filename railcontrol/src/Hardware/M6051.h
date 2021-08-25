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
			M6051() = delete;
			M6051(const M6051&) = delete;
			M6051& operator=(const M6051&) = delete;

			M6051(const HardwareParams* params);
			~M6051();

			inline Hardware::Capabilities GetCapabilities() const override
			{
				return Hardware::CapabilityLoco
					| Hardware::CapabilityAccessory
					| Hardware::CapabilityFeedback;
			}

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolMM2); }

			bool LocoProtocolSupported(Protocol protocol) const override { return (protocol == ProtocolMM2); }

			void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolMM2); }

			bool AccessoryProtocolSupported(Protocol protocol) const override { return (protocol == ProtocolMM2); }

			static void GetArgumentTypesAndHint(std::map<unsigned char,ArgumentType>& argumentTypes, std::string& hint)
			{
				argumentTypes[1] = ArgumentTypeSerialPort;
				argumentTypes[2] = ArgumentTypeS88Modules;
				hint = Languages::GetText(Languages::TextHintM6051);
			}

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoOrientation(const Protocol protocol, const Address address, const Orientation orientation) override;

			void LocoFunction(const Protocol protocol,
				const Address address,
				const DataModel::LocoFunctionNr function,
				const DataModel::LocoFunctionState on) override;

			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;

		private:
			static const unsigned char MaxS88Modules = 62;
			Network::Serial serialLine;
			volatile bool run;
			unsigned char s88Modules;
			std::thread s88Thread;
			unsigned char s88Memory[MaxS88Modules];
			std::map<Address, unsigned char> speedMap;
			std::map<Address, unsigned char> functionMap;

			unsigned char GetSpeedMapEntry(Address address)
			{
				return speedMap.count(address) == 0 ? 0 : speedMap[address];
			}

			unsigned char GetFunctionMapEntry(Address address)
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
} // namespace

