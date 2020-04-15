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
#include <mutex>
#include <vector>

#include "ControlInterface.h"
#include "Logger/Logger.h"
#include "Manager.h"
#include "Network/TcpServer.h"

namespace WebServer
{
	class WebClient;

	class WebServer : public ControlInterface, private Network::TcpServer
	{
		public:
			WebServer() = delete;
			WebServer(Manager& manager, const unsigned short port);
			~WebServer();

			void Work(Network::TcpConnection* connection) override;

			bool NextUpdate(unsigned int& updateIDClient, std::string& s);

			const std::string GetName() const override { return "Webserver"; }
			void AccessoryDelete(const accessoryID_t accessoryID, const std::string& name) override;
			void AccessorySettings(const accessoryID_t accessoryID, const std::string& name) override;
			void AccessoryState(const controlType_t controlType, const DataModel::Accessory* accessory, const accessoryState_t state) override;
			void Booster(const controlType_t controlType, const boosterState_t status) override;
			void FeedbackDelete(const feedbackID_t feedbackID, const std::string& name) override;
			void FeedbackSettings(const feedbackID_t feedbackID, const std::string& name) override;
			void FeedbackState(const std::string& name, const feedbackID_t feedbackID, const DataModel::Feedback::feedbackState_t state) override;
			void LayerDelete(const layerID_t layerID, const std::string& name) override;
			void LayerSettings(const layerID_t layerID, const std::string& name) override;
			void LocoDelete(const locoID_t locoID, const std::string& name) override;
			void LocoDestinationReached(const locoID_t locoID, const streetID_t streetID, const trackID_t trackID) override;
			void LocoDirection(const controlType_t controlType, const DataModel::Loco* loco, const direction_t direction) override;
			void LocoFunction(const controlType_t controlType, const DataModel::Loco* loco, const function_t function, const bool on) override;
			void LocoIntoTrack(const locoID_t locoID, const trackID_t trackID, const std::string& locoName, const std::string& trackName) override;
			void LocoRelease(const locoID_t locoID) override;
			void LocoSettings(const locoID_t locoID, const std::string& name) override;
			void LocoSpeed(const controlType_t controlType, const DataModel::Loco* loco, const locoSpeed_t speed) override;
			void LocoStart(const locoID_t locoID, const std::string& name) override;
			void LocoStop(const locoID_t locoID, const std::string& name) override;
			void StreetDelete(const streetID_t streetID, const std::string& name) override;
			void StreetRelease(const streetID_t streetID) override;
			void StreetSettings(const streetID_t streetID, const std::string& name) override;
			void SwitchDelete(const switchID_t switchID, const std::string& name) override;
			void SwitchSettings(const switchID_t switchID, const std::string& name) override;
			void SwitchState(const controlType_t controlType, const DataModel::Switch* mySwitch, const switchState_t state) override;
			void TrackDelete(const trackID_t trackID, const std::string& name) override;
			void TrackSettings(const trackID_t trackID, const std::string& name) override;
			void TrackState(const trackID_t trackID, const std::string& name, const bool occupied, const bool blocked, const direction_t direction, const std::string& locoName) override;
			void SignalDelete(const signalID_t signalID, const std::string& name) override;
			void SignalSettings(const signalID_t signalID, const std::string& name) override;
			void SignalState(const controlType_t controlType, const DataModel::Signal* signal, const signalState_t state) override;
			void ProgramDccValue(const CvNumber cv, const CvValue value) override;

		private:
			template<typename... Args> void AddUpdate(const std::string& command, const Languages::textSelector_t text, Args... args)
			{
				AddUpdate(command, Logger::Logger::Format(Languages::GetText(text), args...));
			}
			void AddUpdate(const std::string& command, const std::string& status);
			std::string GetStatus(Languages::textSelector_t status) { return updateStatus + Languages::GetText(status); }

			volatile bool run;
			unsigned int lastClientID;
			std::vector<WebClient*> clients;
			Manager& manager;

			std::map<unsigned int,std::string> updates;
			std::mutex updateMutex;
			unsigned int updateID;
			const unsigned int MaxUpdates = 10;
			const std::string updateStatus = "data: status=";
	};
}; // namespace webserver

