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
			WebServer(const WebServer&) = delete;
			WebServer& operator=(const WebServer&) = delete;

			WebServer(Manager& manager, const std::string& webserveraddress, const unsigned short port);
			~WebServer();

			void Work(Network::TcpConnection* connection) override;

			bool NextUpdate(unsigned int& updateIDClient, std::string& s);

			inline const std::string& GetName() const override
			{
				return Webserver;
			}

			void AccessoryDelete(const AccessoryID accessoryID, const std::string& name) override;
			void AccessorySettings(const AccessoryID accessoryID, const std::string& name) override;
			void AccessoryState(const ControlType controlType, const DataModel::Accessory* accessory) override;
			void Booster(const ControlType controlType, const BoosterState status) override;
			void FeedbackDelete(const FeedbackID feedbackID, const std::string& name) override;
			void FeedbackSettings(const FeedbackID feedbackID, const std::string& name) override;
			void FeedbackState(const std::string& name, const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state) override;
			void LayerDelete(const LayerID layerID, const std::string& name) override;
			void LayerSettings(const LayerID layerID, const std::string& name) override;
			void LocoDelete(const LocoID locoID, const std::string& name) override;
			void LocoDestinationReached(const DataModel::Loco* loco, const DataModel::Route* route, const DataModel::TrackBase* track) override;
			void LocoOrientation(const ControlType controlType, const DataModel::Loco* loco, const Orientation direction) override;

			void LocoFunction(const ControlType controlType,
				const DataModel::Loco* loco,
				const DataModel::LocoFunctionNr function,
				const DataModel::LocoFunctionState on) override;

			void LocoRelease(const LocoID locoID) override;
			void LocoSettings(const LocoID locoID, const std::string& name, const std::string& matchKey) override;
			void LocoSpeed(const ControlType controlType, const DataModel::Loco* loco, const Speed speed) override;
			void LocoStart(const LocoID locoID, const std::string& name) override;
			void LocoStop(const LocoID locoID, const std::string& name) override;
			void RouteDelete(const RouteID routeID, const std::string& name) override;
			void RouteRelease(const RouteID routeID) override;
			void RouteSettings(const RouteID routeID, const std::string& name) override;
			void SwitchDelete(const SwitchID switchID, const std::string& name) override;
			void SwitchSettings(const SwitchID switchID, const std::string& name) override;
			void SwitchState(const ControlType controlType, const DataModel::Switch* mySwitch) override;
			void TrackDelete(const TrackID trackID, const std::string& name) override;
			void TrackSettings(const TrackID trackID, const std::string& name) override;
			void TrackState(const DataModel::Track* track) override;
			void SignalDelete(const SignalID signalID, const std::string& name) override;
			void SignalSettings(const SignalID signalID, const std::string& name) override;
			void SignalState(const ControlType controlType, const DataModel::Signal* signal) override;
			void ClusterDelete(const ClusterID clusterID, const std::string& name) override;
			void ClusterSettings(const ClusterID clusterID, const std::string& name) override;
			void TextDelete(const ClusterID clusterID, const std::string& name) override;
			void TextSettings(const ClusterID clusterID, const std::string& name) override;
			void ProgramValue(const CvNumber cv, const CvValue value) override;

		private:
			template<typename... Args>
			inline void AddUpdate(const std::string& command, const Languages::TextSelector text, Args... args)
			{
				AddUpdate(command, Logger::Logger::Format(Languages::GetText(text), args...));
			}

			void AddUpdate(const std::string& command, const std::string& status);

			inline std::string GetStatus(Languages::TextSelector status)
			{
				return UpdateStatus + Languages::GetText(status);
			}

			void TrackBaseState(std::string& command, const DataModel::TrackBase* track);

			void LogBrowserInfo(const std::string& webserveraddress, const unsigned short port);

			Logger::Logger* logger;
			volatile bool run;
			unsigned int lastClientID;
			std::vector<WebClient*> clients;
			Manager& manager;

			std::map<unsigned int,std::string> updates;
			std::mutex updateMutex;
			unsigned int updateID;

			static const unsigned int MaxUpdates = 10;
			static const std::string UpdateStatus;
			static const std::string Webserver;
	};
} // namespace WebServer

