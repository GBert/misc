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

#include <string>

namespace Network
{
	class TcpConnection
	{
		public:
			TcpConnection() = delete;
			TcpConnection(int socket)
			:	connectionSocket(socket),
				connected(socket != 0)
			{}

			~TcpConnection()
			{
				Terminate();
			}

			void Terminate();
			int Send(const char* buf, const size_t buflen, const int flags = 0);
			int Send(const std::string& string, const int flags = 0)
			{
				return Send(string.c_str(), string.size(), flags);
			}

			int Receive(char* buf, const size_t buflen, const int flags = 0);

			bool IsConnected() const { return connected; }

		private:
			int connectionSocket;
			volatile bool connected;
	};
}
