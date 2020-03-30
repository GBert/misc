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

			controlID_t ControlID() const { return params->GetControlID(); }
			const std::string GetName() const override;

			void AccessoryProtocols(std::vector<protocol_t>& protocols) const override;
			bool AccessoryProtocolSupported(protocol_t protocol) const override;
			void AccessoryState(const controlType_t controlType, const DataModel::Accessory* accessory, const accessoryState_t state) override;

			void Booster(const controlType_t controlType, boosterState_t status) override;
			bool CanHandleAccessories() const override;
			bool CanHandleFeedbacks() const override;
			bool CanHandleLocos() const override;
			void LocoDirection(const controlType_t controlType, const DataModel::Loco* loco, const direction_t direction) override;
			void LocoFunction(const controlType_t controlType, const DataModel::Loco* loco, const function_t function, const bool on) override;
			void LocoProtocols(std::vector<protocol_t>& protocols) const override;
			bool LocoProtocolSupported(protocol_t protocol) const override;
			void LocoSpeed(const controlType_t controlType, const DataModel::Loco* loco, const locoSpeed_t speed) override;
			void LocoSpeedDirectionFunctions(const DataModel::Loco* loco, const locoSpeed_t speed, const direction_t direction, std::vector<bool>& functions) override;
			void SwitchState(const controlType_t controlType, const DataModel::Switch* mySwitch, const switchState_t state) override;
			void SignalState(const controlType_t controlType, const DataModel::Signal* signal, const signalState_t state) override;

			static void ArgumentTypesOfHardwareType(const hardwareType_t hardwareType, std::map<unsigned char,argumentType_t>& arguments);

		private:
			Manager& manager;
			createHardware_t* createHardware;
			destroyHardware_t* destroyHardware;
			Hardware::HardwareInterface* instance;
			const HardwareParams* params;

			static const std::string hardwareSymbols[];

			void Init(const HardwareParams* params);
			void Close();
	};
}; // namespace Hardware

