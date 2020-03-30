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
#include <cstring>    // memset
#include <unistd.h>   // close & TEMP_FAILURE_RETRY;

#include "Network/Select.h"
#include "Network/UdpConnection.h"
#include "Utils/Utils.h"

namespace Network
{
	UdpConnection::UdpConnection(Logger::Logger* logger, const std::string& server, const unsigned short port)
	:	logger(logger),
	 	connected(false),
	 	port(port)
	{
		memset((char*)&sockaddr, 0, sizeof(sockaddr));
		struct sockaddr_in* sockaddr_in = reinterpret_cast<struct sockaddr_in*>(&sockaddr);
		sockaddr_in->sin_family = AF_INET;
		sockaddr_in->sin_port = htons(port);
		int ok = inet_pton(AF_INET, server.c_str(), &sockaddr_in->sin_addr);
		if (ok <= 0)
		{
			logger->Error(Languages::TextUnableToResolveAddress, server);
			return;
		}

		// create socket
		connectionSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (connectionSocket == -1)
		{
			logger->Error(Languages::TextUnableToCreateUdpSocket, server, port);
			return;
		}

		// setting receive timeout to 1s
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		setsockopt(connectionSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(struct timeval));
		connected = true;
	}

	bool UdpConnection::Bind()
	{
		int ret = bind(connectionSocket, &sockaddr, sizeof(sockaddr));
		if (ret == 0)
		{
			return true;
		}

		logger->Error(Languages::TextUnableToBindSocketToPort, port);
		connected = false;
		close(connectionSocket);
		return false;
	}

	void UdpConnection::Terminate()
	{
		if (connected)
		{
			connected = false;
			close(connectionSocket);
		}
	}

	int UdpConnection::Send(const char* buffer, const size_t bufferLength)
	{
		if (!connected)
		{
			logger->Error(Languages::TextConnectionReset);
			errno = ECONNRESET;
			return -1;
		}
		return sendto(connectionSocket, buffer, bufferLength, 0, &sockaddr, sizeof(struct sockaddr));
	}

	int UdpConnection::Receive(char* buffer, const size_t bufferLength)
	{
		if (!connected)
		{
			logger->Error(Languages::TextConnectionReset);
			errno = ECONNRESET;
			return -1;
		}
		ssize_t ret;
		do
		{
			if (!connected)
			{
				return 0;
			}
			ret = recvfrom(connectionSocket, buffer, bufferLength, 0, NULL, NULL);
		} while(ret < 0 && errno == EAGAIN);
		return ret;
	}
}
