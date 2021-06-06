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

#include <arpa/inet.h>
#include <unistd.h>   // close & TEMP_FAILURE_RETRY;

#include "Network/Select.h"
#include "Network/TcpConnection.h"
#include "Utils/Utils.h"

namespace Network
{
	void TcpConnection::Terminate() const
	{
		if (connected)
		{
			connected = false;
			close(connectionSocket);
		}
	}

	int TcpConnection::Send(const unsigned char* buffer, const size_t bufferLength, const int flags) const
	{
		if (connectionSocket == 0 || connected == false)
		{
			errno = ENOTCONN;
			return -1;
		}
		errno = 0;
		fd_set set;
		FD_ZERO(&set);
		FD_SET(connectionSocket, &set);
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		int ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, NULL, &set, NULL, &timeout));
		if (ret < 0)
		{
			return ret;
		}
		if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
		ret = send(connectionSocket, buffer, bufferLength, flags | MSG_NOSIGNAL);
		if (ret <= 0)
		{
			errno = ECONNRESET;
			Terminate();
			return -1;
		}
		return ret;
	}

	int TcpConnection::Receive(unsigned char* buffer, const size_t buffferLength, const int flags) const
	{
		if (connectionSocket == 0 || connected == false)
		{
			errno = ENOTCONN;
			return -1;
		}
		errno = 0;
		fd_set set;
		FD_ZERO(&set);
		FD_SET(connectionSocket, &set);
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &timeout));
		if (ret < 0)
		{
			return ret;
		}
		if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
		ret = recv(connectionSocket, buffer, buffferLength, flags);
		if (ret <= 0)
		{
			errno = ECONNRESET;
			Terminate();
			return -1;
		}
		return ret;
	}

	int TcpConnection::ReceiveExact(unsigned char* data, const size_t length, const int flags) const
	{
		int actualSize = 0;
		int endSize = length;
		while (actualSize < endSize)
		{
			int ret = Receive(data + actualSize, endSize - actualSize, flags);
			if (ret <= 0)
			{
				return actualSize;
			}
			actualSize += ret;
		}
		return actualSize;
	}
}
