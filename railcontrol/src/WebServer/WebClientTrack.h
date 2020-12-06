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

#include "Logger/Logger.h"
#include "Manager.h"
#include "WebServer/WebClientTrackBase.h"

namespace WebServer
{
	class WebClient;

	class WebClientTrack : WebClientTrackBase
	{
		public:
			WebClientTrack() = delete;

			inline WebClientTrack(Manager& manager, WebClient& client, Logger::Logger* logger)
			:	manager(manager),
				client(client),
				logger(logger)
			{}

			void HandleTrackEdit(const std::map<std::string,std::string>& arguments);
			void HandleTrackSave(const std::map<std::string,std::string>& arguments);
			void HandleTrackList();
			void HandleTrackAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleTrackDelete(const std::map<std::string,std::string>& arguments);
			void HandleTrackGet(const std::map<std::string, std::string>& arguments);
			void HandleTrackSetLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackRelease(const std::map<std::string, std::string>& arguments);
			void HandleTrackStartLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackStopLoco(const std::map<std::string, std::string>& arguments);
			void HandleTrackBlock(const std::map<std::string, std::string>& arguments);
			void HandleTrackOrientation(const std::map<std::string, std::string>& arguments);

		private:
			Manager& manager;
			WebClient& client;
			Logger::Logger* logger;
	};

} // namespace WebServer

