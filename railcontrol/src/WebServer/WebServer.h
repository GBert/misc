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
#include <sstream>
#include <vector>

#include "ControlInterface.h"
#include "DataModel/AccessoryBase.h"
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
			void AccessoryDelete(const AccessoryID accessoryID, const std::string& name) override;
			void AccessorySettings(const AccessoryID accessoryID, const std::string& name) override;
			void AccessoryState(const ControlType controlType, const DataModel::Accessory* accessory, const DataModel::AccessoryState state) override;
			void Booster(const ControlType controlType, const BoosterState status) override;
			void FeedbackDelete(const FeedbackID feedbackID, const std::string& name) override;
			void FeedbackSettings(const FeedbackID feedbackID, const std::string& name) override;
			void FeedbackState(const std::string& name, const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state) override;
			void LayerDelete(const LayerID layerID, const std::string& name) override;
			void LayerSettings(const LayerID layerID, const std::string& name) override;
			void LocoDelete(const LocoID locoID, const std::string& name) override;
			void LocoDestinationReached(const DataModel::Loco* loco, const DataModel::Street* street, const DataModel::TrackBase* track) override;
			void LocoDirection(const ControlType controlType, const DataModel::Loco* loco, const Direction direction) override;
			void LocoFunction(const ControlType controlType, const DataModel::Loco* loco, const Function function, const DataModel::LocoFunctions::FunctionState on) override;
			void LocoRelease(const LocoID locoID) override;
			void LocoSettings(const LocoID locoID, const std::string& name) override;
			void LocoSpeed(const ControlType controlType, const DataModel::Loco* loco, const Speed speed) override;
			void LocoStart(const LocoID locoID, const std::string& name) override;
			void LocoStop(const LocoID locoID, const std::string& name) override;
			void StreetDelete(const StreetID streetID, const std::string& name) override;
			void StreetRelease(const StreetID streetID) override;
			void StreetSettings(const StreetID streetID, const std::string& name) override;
			void SwitchDelete(const SwitchID switchID, const std::string& name) override;
			void SwitchSettings(const SwitchID switchID, const std::string& name) override;
			void SwitchState(const ControlType controlType, const DataModel::Switch* mySwitch, const DataModel::AccessoryState state) override;
			void TrackDelete(const TrackID trackID, const std::string& name) override;
			void TrackSettings(const TrackID trackID, const std::string& name) override;
			void TrackState(const DataModel::Track* track) override;
			void SignalDelete(const SignalID signalID, const std::string& name) override;
			void SignalSettings(const SignalID signalID, const std::string& name) override;
			void SignalState(const ControlType controlType, const DataModel::Signal* signal) override;
			void ProgramValue(const CvNumber cv, const CvValue value) override;

		private:
			template<typename... Args> void AddUpdate(const std::string& command, const Languages::TextSelector text, Args... args)
			{
				AddUpdate(command, Logger::Logger::Format(Languages::GetText(text), args...));
			}
			void AddUpdate(const std::string& command, const std::string& status);
			std::string GetStatus(Languages::TextSelector status) { return updateStatus + Languages::GetText(status); }

			void TrackBaseState(std::stringstream& command, const DataModel::TrackBase* track);

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

