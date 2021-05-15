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

namespace WebServer
{
	class WebClient;

	class WebClientText
	{
		public:
			WebClientText() = delete;
			WebClientText(const WebClientText&) = delete;
			WebClientText& operator=(const WebClientText&) = delete;

			inline WebClientText(Manager& manager, WebClient& client)
			:	manager(manager),
				client(client)
			{
			}

			void HandleTextEdit(const std::map<std::string,std::string>& arguments);
			void HandleTextSave(const std::map<std::string,std::string>& arguments);
			void HandleTextList();
			void HandleTextAskDelete(const std::map<std::string,std::string>& arguments);
			void HandleTextDelete(const std::map<std::string,std::string>& arguments);
			void HandleTextGet(const std::map<std::string, std::string>& arguments);

		private:
			Manager& manager;
			WebClient& client;
	};
} // namespace WebServer

