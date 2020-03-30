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

#include <arpa/inet.h>
#include <string>

#include "Logger/Logger.h"

namespace Network
{
	class UdpConnection
	{
		public:
			UdpConnection() = delete;
			UdpConnection(Logger::Logger* logger, const std::string& server, const unsigned short port);

			~UdpConnection() { Terminate(); }

			void Terminate();

			bool Bind();

			bool IsConnected() { return connected; }

			int Send(const char* buffer, const size_t bufferLength);
			int Send(unsigned const char* buffer, const size_t bufferLength) { return Send(reinterpret_cast<const char*>(buffer), bufferLength); }
			int Send(const std::string& string) { return Send(string.c_str(), string.size()); }

			int Receive(char* buffer, const size_t bufferLength);
			int Receive(unsigned char* buffer, const size_t bufferLength) { return Receive(reinterpret_cast<char*>(buffer), bufferLength); }

		private:
			Logger::Logger* logger;
			int connectionSocket;
			volatile bool connected;
			struct sockaddr sockaddr;
			volatile bool run;
			const unsigned short port;
	};
}
