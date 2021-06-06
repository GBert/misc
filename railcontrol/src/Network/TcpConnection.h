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

namespace Network
{
	class TcpConnection
	{
		public:
			TcpConnection() = delete;
			TcpConnection& operator=(const TcpConnection&) = delete;

			inline TcpConnection(int socket)
			:	connectionSocket(socket),
				connected(socket != 0)
			{
			}

			inline ~TcpConnection()
			{
				Terminate();
			}

			void Terminate() const;

			int Send(const unsigned char* buffer, const size_t bufferLength, const int flags = 0) const;

			inline int Send(const char* buffer, const size_t bufferLength, const int flags = 0) const
			{
				return Send(reinterpret_cast<const unsigned char*>(buffer), bufferLength, flags);
			}

			inline int Send(const std::string& string, const int flags = 0) const
			{
				return Send(string.c_str(), string.size(), flags);
			}

			int Receive(unsigned char* buffer, const size_t bufferLength, const int flags = 0) const;

			inline int Receive(char* buffer, const size_t bufferLength, const int flags = 0) const
			{
				return Receive(reinterpret_cast<unsigned char*>(buffer), bufferLength, flags);
			}

			int ReceiveExact(unsigned char* buffer, const size_t bufferLength, const int flags = 0) const;

			inline bool IsConnected() const
			{
				return connected;
			}

		private:
			mutable int connectionSocket;
			mutable volatile bool connected;
	};
}
