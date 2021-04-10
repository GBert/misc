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

#include <string>
#include <thread>
#include <vector>

#include "Network/TcpConnection.h"

struct sockaddr;

namespace Network
{
	class TcpServer
	{
		public:
			TcpServer() = delete;
			TcpServer(const TcpServer&) = delete;
			TcpServer& operator=(const TcpServer&) = delete;

		protected:
			TcpServer(const unsigned short port, const std::string& threadName);
			virtual ~TcpServer();
			void TerminateTcpServer();

			virtual void Work(Network::TcpConnection* connection) = 0;

		private:
			void SocketCreateBindListen(int family, struct sockaddr* address);
			void Worker(int socket);

			volatile bool run;
			std::vector<std::thread> serverThreads;
			std::string error;
			const std::string threadName;
	};
}
