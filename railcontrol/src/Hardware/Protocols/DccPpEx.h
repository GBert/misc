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

#include "Hardware/Capabilities.h"
#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Logger/Logger.h"

namespace Hardware
{
	namespace Protocols
	{
		class DccPpEx: protected HardwareInterface
		{
			public:
				DccPpEx() = delete;
				DccPpEx(const DccPpEx&) = delete;
				DccPpEx& operator=(const DccPpEx&) = delete;

				inline Hardware::Capabilities GetCapabilities() const override
				{
					return CapabilityNone;
				}

				void GetLocoProtocols(std::vector<Protocol>& protocols) const override
				{
					protocols.push_back(ProtocolDCC28);
					protocols.push_back(ProtocolDCC128);
				}

				inline bool LocoProtocolSupported(Protocol protocol) const override
				{
					return (protocol == ProtocolDCC28 || protocol == ProtocolDCC128);
				}

				inline void GetAccessoryProtocols(std::vector<Protocol>& protocols) const override
				{
					protocols.push_back(ProtocolDCC);
				}

				inline bool AccessoryProtocolSupported(Protocol protocol) const override
				{
					return (protocol == ProtocolDCC);
				}

				void Booster(const BoosterState status) override;

				void LocoSpeed(const Protocol protocol,
					const Address address,
					const Speed speed) override;

				void LocoOrientation(const Protocol protocol,
					const Address address,
					const Orientation orientation)
				    override;

				void LocoFunction(const Protocol protocol,
				    const Address address,
				    const DataModel::LocoFunctionNr function,
				    const DataModel::LocoFunctionState on) override;

				void AccessoryOnOrOff(const Protocol protocol,
					const Address address,
				    const DataModel::AccessoryState state,
				    const bool on) override;

				void ProgramWrite(const ProgramMode mode,
					const Address address,
					const CvNumber cv,
					const CvValue value) override;

			protected:
				inline DccPpEx(const HardwareParams* params,
				    const std::string& fullName,
				    const std::string& shortName)
				:	HardwareInterface(params->GetManager(),
						params->GetControlID(),
						fullName,
						shortName)
				{
				}

				virtual ~DccPpEx()
				{
				}

			private:
				inline void SendInternal(const std::string& buffer)
				{
					logger->Hex(buffer);
					Send(buffer);
				}
				virtual void Send(const std::string& buffer) = 0;
		};
	} // namespace
} // namespace
