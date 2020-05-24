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
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Network/TcpClient.h"

namespace Network
{
	TcpConnection TcpClient::GetTcpClientConnection(Logger::Logger* logger, const std::string& host, const unsigned short port)
	{
	    struct sockaddr_in address;
	    address.sin_family = AF_INET;
	    address.sin_port = htons(port);
	    int ok = inet_pton(AF_INET, host.c_str(), &address.sin_addr);
	    if (ok <= 0)
	    {
			logger->Error(Languages::TextUnableToResolveAddress, host);
	        return TcpConnection(0);
	    }

	    int sock = socket(AF_INET, SOCK_STREAM, 0);
	    if (sock < 0)
	    {
			logger->Error(Languages::TextUnableToCreateTcpSocket, host, port);
	        return TcpConnection(0);
	    }

	    ok = connect(sock, (struct sockaddr *)&address, sizeof(address));
	    if (ok < 0)
	    {
	    	Languages::TextSelector text;
	    	switch (errno)
	    	{
	    		case ECONNREFUSED:
	    			text = Languages::TextConnectionRefused;
	    			break;

	    		case ENETUNREACH:
	    			text = Languages::TextNetworkUnreachable;
	    			break;

	    		default:
	    			text = Languages::TextConnectionFailed;
		    }
			logger->Error(text, host, port);
	        close(sock);
	        return TcpConnection(0);
	    }

		return TcpConnection(sock);
	}
}
