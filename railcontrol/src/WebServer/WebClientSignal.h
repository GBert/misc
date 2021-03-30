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
#include <string>

#include "Logger/Logger.h"
#include "Manager.h"
#include "WebServer/WebClientTrackBase.h"

namespace WebServer
{
	class WebClient;

	class WebClientSignal : WebClientTrackBase
	{
		public:
			WebClientSignal() = delete;
			WebClientSignal(const WebClientSignal&) = delete;
			WebClientSignal& operator=(const WebClientSignal&) = delete;

			inline WebClientSignal(Manager& manager, WebClient& client, Logger::Logger* logger)
			:	manager(manager),
				client(client),
				logger(logger)
			{}

			void HandleSignalEdit(const std::map<std::string,std::string>& arguments);
			void HandleSignalSave(const std::map<std::string,std::string>& arguments);
			void HandleSignalList();
			void HandleSignalAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleSignalDelete(const std::map<std::string,std::string>& arguments);
			void HandleSignalGet(const std::map<std::string, std::string>& arguments);
			void HandleSignalSetLoco(const std::map<std::string, std::string>& arguments);
			void HandleSignalRelease(const std::map<std::string, std::string>& arguments);
			void HandleSignalState(const std::map<std::string, std::string>& arguments);
			void HandleSignalStates(const std::map<std::string, std::string>& arguments);

			HtmlTag HtmlTagRelationSignalState(const std::string& name,
				const SignalID signalId,
				const DataModel::Relation::Data data = DataModel::SignalStateStop);

		private:
			Manager& manager;
			WebClient& client;
			Logger::Logger* logger;
	};

} // namespace WebServer

