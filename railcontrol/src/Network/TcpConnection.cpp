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

#include <arpa/inet.h>
#include <unistd.h>   // close & TEMP_FAILURE_RETRY;

#include "Network/Select.h"
#include "Network/TcpConnection.h"
#include "Utils/Utils.h"

namespace Network
{
	void TcpConnection::Terminate()
	{
		if (connected)
		{
			connected = false;
			close(connectionSocket);
		}
	}

	int TcpConnection::Send(const char* buf, const size_t buflen, const int flags)
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
		ret = send(connectionSocket, buf, buflen, flags | MSG_NOSIGNAL);
		if (ret <= 0)
		{
			errno = ECONNRESET;
			Terminate();
			return -1;
		}
		return ret;
	}

	int TcpConnection::Receive(char* buf, const size_t buflen, const int flags)
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
		ret = recv(connectionSocket, buf, buflen, flags);
		if (ret <= 0)
		{
			errno = ECONNRESET;
			Terminate();
			return -1;
		}
		return ret;
	}
}
