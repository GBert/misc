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

#include "DataModel/Feedback.h"
#include "DataTypes.h"

namespace DataModel
{
	class Accessory;
	class Loco;
	class Signal;
	class Switch;
}

namespace Hardware
{
	class HardwareParams;
}

class ControlInterface
{
	public:
		ControlInterface(controlType_t controlType) : controlType(controlType) {}
		virtual ~ControlInterface() {};
		virtual void ReInit(__attribute__((unused)) const Hardware::HardwareParams* params) {};
		controlType_t ControlType() const { return controlType; }
		virtual const std::string GetName() const = 0;
		virtual void AccessoryDelete(__attribute__((unused)) const accessoryID_t accessoryID, __attribute__((unused)) const std::string& name) {}
		virtual void AccessoryProtocols(__attribute__((unused)) std::vector<protocol_t>& protocols) const {};
		virtual bool AccessoryProtocolSupported(__attribute__((unused)) protocol_t protocol) const { return false; };
		virtual void AccessorySettings(__attribute__((unused)) const accessoryID_t accessoryID, __attribute__((unused)) const std::string& name) {}
		virtual void AccessoryState(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Accessory* accessory, __attribute__((unused)) const accessoryState_t state) {};
		virtual void ArgumentTypes(__attribute__((unused)) std::map<unsigned char,argumentType_t>& argumentTypes) const {}
		virtual void Booster(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const boosterState_t state) {};
		virtual bool CanHandleAccessories() const { return false; }
		virtual bool CanHandleFeedbacks() const { return false; }
		virtual bool CanHandleLocos() const { return false; }
		virtual void FeedbackDelete(__attribute__((unused)) const feedbackID_t feedbackID, __attribute__((unused)) const std::string& name) {}
		virtual void FeedbackSettings(__attribute__((unused)) const feedbackID_t feedbackID, __attribute__((unused)) const std::string& name) {}
		virtual void FeedbackState(__attribute__((unused)) const std::string& name, __attribute__((unused)) const feedbackID_t feedbackID, __attribute__((unused)) const DataModel::Feedback::feedbackState_t state) {};
		virtual void LayerDelete(__attribute__((unused)) const layerID_t layerID, __attribute__((unused)) const std::string& name) {};
		virtual void LayerSettings(__attribute__((unused)) const layerID_t layerID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoDelete(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoDestinationReached(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const streetID_t streetID, __attribute__((unused)) const trackID_t trackID) {};
		virtual void LocoDirection(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const direction_t direction) {};
		virtual void LocoFunction(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const function_t function, __attribute__((unused)) const bool on) {};
		virtual void LocoIntoTrack(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const trackID_t trackID, __attribute__((unused)) const std::string& locoName, __attribute__((unused)) const std::string& trackName) {};
		virtual void LocoProtocols(__attribute__((unused)) std::vector<protocol_t>& protocols) const {};
		virtual bool LocoProtocolSupported(__attribute__((unused)) protocol_t protocol) const { return false; };
		virtual void LocoRelease(__attribute__((unused)) const locoID_t locoID) {};
		virtual void LocoSettings(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoSpeed(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Loco* loco, __attribute__((unused)) const locoSpeed_t speed) {};
		virtual void LocoStart(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const std::string& name) {};
		virtual void LocoStop(__attribute__((unused)) const locoID_t locoID, __attribute__((unused)) const std::string& name) {};
		virtual void StreetDelete(__attribute__((unused)) const streetID_t streetID, __attribute__((unused)) const std::string& name) {};
		virtual void StreetRelease(__attribute__((unused)) const streetID_t streetID) {};
		virtual void StreetSettings(__attribute__((unused)) const streetID_t streetID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchDelete(__attribute__((unused)) const switchID_t switchID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchSettings(__attribute__((unused)) const switchID_t switchID, __attribute__((unused)) const std::string& name) {};
		virtual void SwitchState(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Switch* mySwitch, __attribute__((unused)) const switchState_t state) {};
		virtual void TrackDelete(__attribute__((unused)) const trackID_t trackID, __attribute__((unused)) const std::string& name) {};
		virtual void TrackSettings(__attribute__((unused)) const trackID_t trackID, __attribute__((unused)) const std::string& name) {};
		virtual void TrackState(__attribute__((unused)) const trackID_t trackID, __attribute__((unused)) const std::string& name, __attribute__((unused)) const bool occupied, __attribute__((unused)) const bool blocked, __attribute__((unused)) const direction_t direction, __attribute__((unused)) const std::string& locoName) {};
		virtual void SignalDelete(__attribute__((unused)) const signalID_t signalID, __attribute__((unused)) const std::string& name) {};
		virtual void SignalSettings(__attribute__((unused)) const signalID_t signalID, __attribute__((unused)) const std::string& name) {};
		virtual void SignalState(__attribute__((unused)) const controlType_t controlType, __attribute__((unused)) const DataModel::Signal* signal, __attribute__((unused)) const signalState_t state) {};

		virtual void LocoSpeedDirectionFunctions(const DataModel::Loco* loco, const locoSpeed_t speed, const direction_t direction, std::vector<bool>& functions)
		{
			LocoSpeed(ControlTypeInternal, loco, speed);
			LocoDirection(ControlTypeInternal, loco, direction);
			for (size_t functionNr = 0; functionNr < functions.size(); ++functionNr)
			{
				LocoFunction(ControlTypeInternal, loco, functionNr, functions[functionNr]);
			}
		}

	private:
		controlType_t controlType;
};
