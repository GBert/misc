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

#include <cstring>

#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Logger/Logger.h"

namespace Hardware
{

	class Virtual : HardwareInterface
	{
		public:
			Virtual(const HardwareParams* params);

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }
			bool CanHandleProgram() const override { return true; }
			bool CanHandleProgramMm() const override { return true; }
			bool CanHandleProgramDccDirect() const override { return true; }
			bool CanHandleProgramDccPom() const override { return true; }

			static void GetHint(std::string& hint)
			{
				hint = Languages::GetText(Languages::TextHintVirtual);
			}

			void GetLocoProtocols(std::vector<protocol_t>& protocols) const override { protocols.push_back(ProtocolNone); }
			bool LocoProtocolSupported(const protocol_t protocol) const override { return protocol == ProtocolNone; }
			void GetAccessoryProtocols(std::vector<protocol_t>& protocols) const override { protocols.push_back(ProtocolNone); }
			bool AccessoryProtocolSupported(const protocol_t protocol) const override { return protocol == ProtocolNone; }

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t protocol, const address_t address, const locoSpeed_t speed) override;
			void LocoDirection(const protocol_t protocol, const address_t address, const direction_t direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void AccessoryOnOrOff(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const address_t address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const address_t address, const CvNumber cv, const CvValue value) override;

		private:
			Logger::Logger* logger;
	};

	extern "C" Virtual* create_Virtual(const HardwareParams* params);
	extern "C" void destroy_Virtual(Virtual* virt);

} // namespace

