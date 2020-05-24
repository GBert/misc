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

#include <iostream>

#include "Logger/Logger.h"
#include "Logger/LoggerServer.h"
#include "Utils/Utils.h"

using std::string;

namespace Logger
{
	LoggerServer::~LoggerServer()
	{
		if (run == false)
		{
			return;
		}

		run = false;

		// delete all client memory
		while (clients.size() > 0)
		{
			LoggerClient* client = clients.back();
			clients.pop_back();
			delete client;
		}

		// delete all logger memory
		while (loggers.size() > 0)
		{
			Logger* logger = loggers.back();
			loggers.pop_back();
			delete logger;
		}
	}

	Logger* LoggerServer::GetLogger(const std::string& component)
	{
		for (auto logger : loggers)
		{
			if (logger->IsComponent(component))
			{
				return logger;
			}
		}
		Logger* logger = new Logger(*this, component);
		loggers.push_back(logger);
		return logger;
	}

	void LoggerServer::Send(const std::string& text)
	{
		for(auto client : clients)
		{
			client->Send(text);
		}
	}
}
