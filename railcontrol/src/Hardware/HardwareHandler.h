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

#include "ControlInterface.h"
#include "DataModel/LocoFunctions.h"
#include "DataTypes.h"
#include "Hardware/HardwareInterface.h"
#include "Hardware/HardwareParams.h"
#include "Manager.h"

namespace Hardware
{
	// the types of the class factories
	typedef Hardware::HardwareInterface* createHardware_t(const Hardware::HardwareParams* params);
	typedef void destroyHardware_t(Hardware::HardwareInterface*);

	class HardwareHandler: public ControlInterface
	{
		public:
			HardwareHandler(Manager& manager, const HardwareParams* params)
			:	ControlInterface(ControlTypeHardware),
				manager(manager),
				createHardware(nullptr),
				destroyHardware(nullptr),
				instance(nullptr),
				params(nullptr)
			{
				Init(params);
			}

			~HardwareHandler()
			{
				Close();
			}

			void ReInit(const HardwareParams* params)
			{
				Close();
				Init(params);
			}

			ControlID GetControlID() const { return params->GetControlID(); }
			const std::string GetName() const override;

			void AccessoryProtocols(std::vector<Protocol>& protocols) const override;
			bool AccessoryProtocolSupported(Protocol protocol) const override;
			void AccessoryState(const ControlType controlType, const DataModel::Accessory* accessory, const DataModel::AccessoryState state) override;

			void Booster(const ControlType controlType, BoosterState status) override;
			bool CanHandleAccessories() const override;
			bool CanHandleFeedbacks() const override;
			bool CanHandleLocos() const override;
			bool CanHandleProgram() const override;
			bool CanHandleProgramMm() const override;
			bool CanHandleProgramMfx() const override;
			bool CanHandleProgramDccDirect() const override;
			bool CanHandleProgramDccPom() const override;
			void LocoDirection(const ControlType controlType, const DataModel::Loco* loco, const Direction direction) override;
			void LocoFunction(const ControlType controlType, const DataModel::Loco* loco, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void LocoProtocols(std::vector<Protocol>& protocols) const override;
			bool LocoProtocolSupported(Protocol protocol) const override;
			void LocoSpeed(const ControlType controlType, const DataModel::Loco* loco, const Speed speed) override;
			void LocoSpeedDirectionFunctions(const DataModel::Loco* loco, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions) override;
			void SwitchState(const ControlType controlType, const DataModel::Switch* mySwitch, const DataModel::AccessoryState state) override;
			void SignalState(const ControlType controlType, const DataModel::Signal* signal) override;
			void ProgramRead(const ProgramMode mode, const Address address, const CvNumber cv) override;
			void ProgramWrite(const ProgramMode mode, const Address address, const CvNumber cv, const CvValue value) override;

			static void ArgumentTypesOfHardwareTypeAndHint(const HardwareType hardwareType, std::map<unsigned char,ArgumentType>& arguments, std::string& hint);

		private:
			Manager& manager;
			createHardware_t* createHardware;
			destroyHardware_t* destroyHardware;
			Hardware::HardwareInterface* instance;
			const HardwareParams* params;

			static const std::string hardwareSymbols[];

			void Init(const HardwareParams* params);
			void Close();
			bool ProgramCheckValues(const ProgramMode mode, const CvNumber cv, const CvValue value = 1);
	};
}; // namespace Hardware

