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

#include <cstring>		//memset
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "Network/Select.h"
#include "Network/TcpServer.h"
#include "Utils/Utils.h"

namespace Network
{
	TcpServer::TcpServer(const unsigned short port, const std::string& threadName)
	:	run(false),
	 	error(""),
	 	threadName(threadName)
	{
		struct sockaddr_in6 serverAddr6;
		memset(reinterpret_cast<char*>(&serverAddr6), 0, sizeof(serverAddr6));
		serverAddr6.sin6_family = AF_INET6;
		serverAddr6.sin6_addr = in6addr_any;
		serverAddr6.sin6_port = htons(port);
		SocketCreateBindListen(serverAddr6.sin6_family, reinterpret_cast<struct sockaddr*>(&serverAddr6));

#ifdef __CYGWIN__
		struct sockaddr_in serverAddr4;
		memset(reinterpret_cast<char*>(&serverAddr4), 0, sizeof(serverAddr4));
		serverAddr4.sin_family = AF_INET;
		serverAddr4.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr4.sin_port = htons(port);
		SocketCreateBindListen(serverAddr4.sin_family, reinterpret_cast<struct sockaddr*>(&serverAddr4));
#endif
	}

	TcpServer::~TcpServer()
	{
		TerminateTcpServer();

		while (connections.size())
		{
			TcpConnection* client = connections.back();
			connections.pop_back();
			delete client;
		}
	}

	void TcpServer::SocketCreateBindListen(int family, struct sockaddr* address)
	{
		int serverSocket = socket(family, SOCK_STREAM, 0);
		if (serverSocket < 0)
		{
			error = "Unable to create socket for tcp server. Unable to serve clients.";
			return;
		}

		int on = 1;
		int intResult = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void*) &on, sizeof(on));
		if (intResult < 0)
		{
			error = "Unable to set tcp server socket option SO_REUSEADDR.";
			close(serverSocket);
			return;
		}

		intResult = bind(serverSocket, address, family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6));
		if (intResult < 0)
		{
			error = "Unable to bind socket for tcp server to port. Unable to serve clients.";
			close (serverSocket);
			return;
		}

		const int MaxClientsInQueue = 5;
		intResult = listen(serverSocket, MaxClientsInQueue);
		if (intResult != 0)
		{
			error = "Unable to listen on socket for tcp server. Unable to serve clients.";
			close(serverSocket);
			return;
		}

		run = true;
		serverThreads.push_back(std::thread(&Network::TcpServer::Worker, this, serverSocket));
	}

	void TcpServer::TerminateTcpServer()
	{
		if (run == false)
		{
			return;
		}

		run = false;

		for(std::thread& serverThread : serverThreads)
		{
			serverThread.join();
		}
	}

	void TcpServer::Worker(int socket)
	{
		Utils::Utils::SetThreadName(threadName);
		fd_set set;
		struct timeval tv;
		struct sockaddr_in6 client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		while (run == true)
		{
			// wait for connection and abort on shutdown
			int ret;
			do
			{
				FD_ZERO(&set);
				FD_SET(socket, &set);
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				ret = TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &tv));

				if (run == false)
				{
					return;
				}
			} while (ret == 0);

			if (ret < 0)
			{
				continue;
			}

			// accept connection
			int socketClient = accept(socket, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len);
			if (socketClient < 0)
			{
				continue;
			}

			// create client and fill into vector
			auto con = new TcpConnection(socketClient);
			connections.push_back(con);
			Work(con);
		}
	}
}
