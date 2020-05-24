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

#include <map>
#include <string>
#include <vector>

#include "DataModel/AccessoryBase.h"
#include "DataModel/Feedback.h"
#include "DataModel/LocoFunctions.h"
#include "DataTypes.h"

namespace DataModel
{
	class Accessory;
	class Loco;
	class Signal;
	class Street;
	class Switch;
	class Track;
}

namespace Hardware
{
	class HardwareParams;
}

class ControlInterface
{
	public:
		ControlInterface(ControlType controlType) : controlType(controlType) {}
		virtual ~ControlInterface() {};
		virtual void ReInit(__attribute__((unused)) const Hardware::HardwareParams* params) {};
		ControlType GetControlType() const { return controlType; }
		virtual const std::string GetName() const = 0;
		virtual void AccessoryDelete(__attribute__((unused)) const AccessoryID accessoryID, __attribute__((unused)) const std::string& name) {}
		virtual void AccessoryProtocols(__attribute__((unused)) std::vector<Protocol>& protocols) const {};
		virtual bool AccessoryProtocolSupported(__attribute__((unused)) Protocol protocol) const { return false; };
		virtual void AccessorySettings(__attribute__((unused)) const AccessoryID accessoryID, __attribute__((unused)) const std::string& name) {}
		virtual void AccessoryState(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Accessory* accessory, __attribute__((unused)) const DataModel::AccessoryState state) {};
		virtual void ArgumentTypes(__attribute__((unused)) std::map<unsigned char,ArgumentType>& argumentTypes) const {}
		virtual void Booster(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const BoosterState state) {};
		virtual bool CanHandleAccessories() const { return false; }
		virtual bool CanHandleFeedbacks() const { return false; }
		virtual bool CanHandleLocos() const { return false; }
		virtual bool CanHandleProgram() const { return false; }
		virtual bool CanHandleProgramMm() const { return false; }
		virtual bool CanHandleProgramMfx() const { return false; }
		virtual bool CanHandleProgramDccDirect() const { return false; }
		virtual bool CanHandleProgramDccPom() const { return false; }
		virtual void FeedbackDelete(__attribute__((unused)) const FeedbackID feedbackID, __attribute__((unused)) const std::string& name) {}
		virtual void FeedbackSettings(__attribute__((unused)) const FeedbackID feedbackID, __attribute__((unused)) const std::string& name) {}
		virtual void FeedbackState(__attribute__((unused)) const std::string& name, __attribute__((unused)) const FeedbackID feedbackID, __attribute__((unused)) const DataModel::Feedback::FeedbackState state) {};
		virtual void LayerDelete(__attribute__((unused)) const LayerID layerID, __attribute__((unused)) const std::string& name) {};
		virtual void LayerSettings(__attribute__((unused)) const LayerID layerID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoDelete(__attribute__((unused)) const LocoID locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoDestinationReached(__attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const DataModel::Street* street, __attribute__((unused)) const DataModel::TrackBase* track) {};
		virtual void LocoDirection(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const Direction direction) {};
		virtual void LocoFunction(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const Function function, __attribute__((unused)) const DataModel::LocoFunctions::FunctionState on) {};
		virtual void LocoProtocols(__attribute__((unused)) std::vector<Protocol>& protocols) const {};
		virtual bool LocoProtocolSupported(__attribute__((unused)) Protocol protocol) const { return false; };
		virtual void LocoRelease(__attribute__((unused)) const LocoID locoID) {};
		virtual void LocoSettings(__attribute__((unused)) const LocoID locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoSpeed(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const Speed speed) {};
		virtual void LocoStart(__attribute__((unused)) const LocoID locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoStop(__attribute__((unused)) const LocoID locoID, __attribute__((unused)) const std::string& name) {};
		virtual void StreetDelete(__attribute__((unused)) const StreetID streetID, __attribute__((unused)) const std::string& name) {};
		virtual void StreetRelease(__attribute__((unused)) const StreetID streetID) {};
		virtual void StreetSettings(__attribute__((unused)) const StreetID streetID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchDelete(__attribute__((unused)) const SwitchID switchID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchSettings(__attribute__((unused)) const SwitchID switchID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchState(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Switch* mySwitch, __attribute__((unused)) const DataModel::AccessoryState state) {};
		virtual void TrackDelete(__attribute__((unused)) const TrackID trackID, __attribute__((unused)) const std::string& name) {};
		virtual void TrackSettings(__attribute__((unused)) const TrackID trackID, __attribute__((unused)) const std::string& name) {};
		virtual void TrackState(__attribute__((unused)) const DataModel::Track* track) {}
		virtual void SignalDelete(__attribute__((unused)) const SignalID signalID, __attribute__((unused)) const std::string& name) {};
		virtual void SignalSettings(__attribute__((unused)) const SignalID signalID, __attribute__((unused)) const std::string& name) {};
		virtual void SignalState(__attribute__((unused)) const ControlType controlType, __attribute__((unused)) const DataModel::Signal* signal) {};

		virtual void LocoSpeedDirectionFunctions(const DataModel::Loco* loco, const Speed speed, const Direction direction, std::vector<DataModel::LocoFunctions::FunctionState>& functions)
		{
			LocoSpeed(ControlTypeInternal, loco, speed);
			LocoDirection(ControlTypeInternal, loco, direction);
			for (size_t functionNr = 0; functionNr < functions.size(); ++functionNr)
			{
				LocoFunction(ControlTypeInternal, loco, functionNr, functions[functionNr]);
			}
		}

		virtual void ProgramRead(__attribute__((unused)) const ProgramMode mode, __attribute__((unused)) const Address address, __attribute__((unused)) const CvNumber cv) {}
		virtual void ProgramWrite(__attribute__((unused)) const ProgramMode mode, __attribute__((unused)) const Address address, __attribute__((unused)) const CvNumber cv, __attribute__((unused)) const CvValue value) {}
		virtual void ProgramValue(__attribute__((unused)) const CvNumber cv, __attribute__((unused)) const CvValue value) {}

	private:
		ControlType controlType;
};
