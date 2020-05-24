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

#include <fstream>
#include <string>
#include <vector>

#include "Logger/LoggerClient.h"
#include "Logger/LoggerClientConsole.h"
#include "Logger/LoggerClientFile.h"
#include "Logger/LoggerClientTcp.h"
#include "Network/TcpServer.h"

namespace Logger
{
	class Logger;

	class LoggerServer: private Network::TcpServer
	{
		public:
			LoggerServer(LoggerServer const &) = delete;
			void operator=(LoggerServer const &) = delete;

			Logger* GetLogger(const std::string& component);
			void Send(const std::string& text);

			static const unsigned short defaultLoggerPort = 2223;
			static LoggerServer& Instance()
			{
				static LoggerServer server;
				return server;
			}

			void AddFileLogger(const std::string& fileName)
			{
				if (fileLoggerStarted == true)
				{
					return;
				}
				clients.push_back(new LoggerClientFile(fileName));
				fileLoggerStarted = true;
			}

			void AddConsoleLogger()
			{
				if (consoleLoggerStarted == true)
				{
					return;
				}
				clients.push_back(new LoggerClientConsole());
				consoleLoggerStarted = true;
			}

		private:
			LoggerServer()
			:	Network::TcpServer(defaultLoggerPort, "Logger"),
			 	run(true),
			 	fileLoggerStarted(false),
			 	consoleLoggerStarted(false)
			{
			}

			~LoggerServer();

			void Work(Network::TcpConnection* connection) override
			{
				clients.push_back(new LoggerClientTcp(connection));
			}


			volatile bool run;
			bool fileLoggerStarted;
			bool consoleLoggerStarted;
			std::vector<LoggerClient*> clients;
			std::vector<Logger*> loggers;
	};
}
