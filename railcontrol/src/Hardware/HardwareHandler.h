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

#include "ControlInterface.h"
#include "DataModel/LocoFunctions.h"
#include "DataTypes.h"
#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Hardware/LocoCache.h"
#include "Manager.h"

namespace Hardware
{
	class HardwareHandler: public ControlInterface
	{
		public:
			HardwareHandler() = delete;
			HardwareHandler(const HardwareHandler&) = delete;
			HardwareHandler& operator=(const HardwareHandler&) = delete;

			inline HardwareHandler(const HardwareParams* params)
			:	ControlInterface(ControlTypeHardware),
				instance(nullptr),
				params(nullptr)
			{
				Init(params);
			}

			inline ~HardwareHandler()
			{
				Close();
			}

			inline void ReInit(const HardwareParams* params) override
			{
				Close();
				Init(params);
			}

			inline ControlID GetControlID() const
			{
				return params->GetControlID();
			}

			const std::string& GetName() const override;

			const std::string& GetShortName() const override;

			void AccessoryProtocols(std::vector<Protocol>& protocols) const override;
			bool AccessoryProtocolSupported(Protocol protocol) const override;
			void AccessoryState(const ControlType controlType, const DataModel::Accessory* accessory) override;

			void Booster(const ControlType controlType, BoosterState status) override;
			Hardware::Capabilities GetCapabilities() const override;
			void LocoOrientation(const ControlType controlType,
				const DataModel::Loco* loco,
				const Orientation orientation) override;

			void LocoFunction(const ControlType controlType,
				const DataModel::Loco* loco,
				const DataModel::LocoFunctionNr function,
				const DataModel::LocoFunctionState on) override;

			void LocoProtocols(std::vector<Protocol>& protocols) const override;
			bool LocoProtocolSupported(Protocol protocol) const override;
			void LocoSpeed(const ControlType controlType, const DataModel::Loco* loco, const Speed speed) override;

			void LocoSpeedOrientationFunctions(const DataModel::Loco* loco,
				const Speed speed,
				const Orientation orientation,
				std::vector<DataModel::LocoFunctionEntry>& functions) override;

			void LocoSettings(const LocoID locoId,
				__attribute__((unused)) const std::string& name,
				const std::string& matchKey) override;

			void SwitchState(const ControlType controlType, const DataModel::Switch* mySwitch) override;
			void SignalState(const ControlType controlType, const DataModel::Signal* signal) override;
			void ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value) override;

			void AddUnmatchedLocos(std::map<std::string,DataModel::LocoConfig>& list) const override;

			std::map<std::string,DataModel::LocoConfig> GetUnmatchedLocos() const override;

			std::map<std::string,DataModel::LocoConfig> GetAllLocos() const override;

			DataModel::LocoConfig GetLocoByMatch(__attribute__((unused)) const std::string& match) const override;

			static void ArgumentTypesOfHardwareTypeAndHint(const HardwareType hardwareType, std::map<unsigned char,ArgumentType>& arguments, std::string& hint);

		private:
			Hardware::HardwareInterface* instance;
			const HardwareParams* params;

			static const std::string Unknown;

			void Init(const HardwareParams* params);
			void Close();
			bool ProgramCheckValues(const ProgramMode mode, const CvNumber cv, const CvValue value = 1);
	};
} // namespace Hardware

