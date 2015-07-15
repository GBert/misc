/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "pickle.h"

/*
 * Connect to port on host
 */
int
ip_connect(const char *hostname, int port)
{
	struct sockaddr_in addr;
	struct hostent *host;
	int sock;

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if ((addr.sin_addr.s_addr = inet_addr(hostname)) == htonl(INADDR_NONE)) {
		if ((host = gethostbyname(hostname)) == NULL) {
			printf("%s: error: gethostbyname failed\n", __func__);
			return -1;
		}
	        addr.sin_addr.s_addr = ((struct in_addr *)host->h_addr_list[0])->s_addr;
	}
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("%s: error: socket failed\n", __func__);
		return -1;
	}
	if ((connect(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
		close(sock);
		printf("%s: error: connect failed\n", __func__);
		return -1;
	}
#if 0
	int optval = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&optval, sizeof(optval)) < 0) {
		close(sock);
		printf("setsockopt(): ERROR!!!\n");
		return -1;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&optval, sizeof(optval)) < 0) {
		close(sock);
		printf("setsockopt(): ERROR!!!\n");
		return -1;
	}
#endif
	fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL));
	return sock;
}       

/*
 * Listen on port on host
 */
int
ip_listen(__attribute__((unused)) const char *hostname, int port)
{
	struct sockaddr_in addr;
	int listen_sock, sock;

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("%s: error: socket failed\n", __func__);
		return -1;
	}
	int optval = 1;
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval)) < 0) {
		close(listen_sock);
		printf("%s: error: setsockopt failed\n", __func__);
		return -1;
	}
	if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(listen_sock);
		printf("%s: error: bind failed\n", __func__);
		return -1;
	}
	if (listen(listen_sock, 1) < 0) {
		close(listen_sock);
		printf("%s: error: listen failed\n", __func__);
		return -1;
	}
	while (1) {
		sock = accept(listen_sock, NULL, NULL);
		if (sock < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			}
			close(listen_sock);
			printf("%s: error: accept failed\n", __func__);
			return -1;
		}
		close(listen_sock);
		fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL));
		return sock;
	}
	/* Not reached */
}
