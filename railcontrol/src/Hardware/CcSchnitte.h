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

#include "Hardware/HardwareParams.h"
#include "Hardware/MaerklinCAN.h"
#include "Logger/Logger.h"
#include "Network/Serial.h"

namespace Hardware
{
	class CcSchnitte : MaerklinCAN
	{
		public:
			CcSchnitte(const HardwareParams* params);
			~CcSchnitte();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<protocol_t> &protocols) const override
			{
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolMFX);
				protocols.push_back(ProtocolDCC);
			}

			bool LocoProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM2 || protocol == ProtocolMFX || protocol == ProtocolDCC);
			}

			void GetAccessoryProtocols(std::vector<protocol_t> &protocols) const override
			{
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolDCC);
			}

			bool AccessoryProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM2 || protocol == ProtocolDCC);
			}

			static void GetArgumentTypes(std::map<unsigned char, argumentType_t> &argumentTypes)
			{
				argumentTypes[1] = SerialPort;
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;

		private:
			Network::Serial serialLine;
			volatile bool run;
			std::thread receiverThread;
			void Receiver();
	};

	extern "C" CcSchnitte* create_CcSchnitte(const HardwareParams *params);
	extern "C" void destroy_CcSchnitte(CcSchnitte *opendcc);

} // namespace

