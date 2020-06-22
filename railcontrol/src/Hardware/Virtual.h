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

			inline Hardware::Capabilities GetCapabilities() const override
			{
				return Hardware::CapabilityLoco
					| Hardware::CapabilityAccessory
					| Hardware::CapabilityFeedback
					| Hardware::CapabilityProgram
					| Hardware::CapabilityProgramMmWrite
					| Hardware::CapabilityProgramMmPomWrite
					| Hardware::CapabilityProgramMfxRead
					| Hardware::CapabilityProgramMfxWrite
					| Hardware::CapabilityProgramDccDirectRead
					| Hardware::CapabilityProgramDccDirectWrite
					| Hardware::CapabilityProgramDccPomRead
					| Hardware::CapabilityProgramDccPomWrite;
			}

			static void GetHint(std::string& hint)
			{
				hint = Languages::GetText(Languages::TextHintVirtual);
			}

			void GetLocoProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolNone); }
			bool LocoProtocolSupported(const Protocol protocol) const override { return protocol == ProtocolNone; }
			void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override { protocols.push_back(ProtocolNone); }
			bool AccessoryProtocolSupported(const Protocol protocol) const override { return protocol == ProtocolNone; }

			void Booster(const BoosterState status) override;
			void LocoSpeed(const Protocol protocol, const Address address, const Speed speed) override;
			void LocoOrientation(const Protocol protocol, const Address address, const Orientation orientation) override;
			void LocoFunction(const Protocol protocol, const Address address, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void AccessoryOnOrOff(const Protocol protocol, const Address address, const DataModel::AccessoryState state, const bool on) override;
			void ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value) override;

		private:
			Logger::Logger* logger;
	};

	extern "C" Virtual* create_Virtual(const HardwareParams* params);
	extern "C" void destroy_Virtual(Virtual* virt);

} // namespace

