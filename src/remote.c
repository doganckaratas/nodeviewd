/**
 * @file   remote.c
 * @brief  remote server utilities
 * @date   23/08/2018
 * @author Dogan Can Karatas
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "remote.h"

int remote_resolve_ip(const char *hostname, char **ip)
{
	struct hostent *h;
	struct in_addr **in_addr_list;
	int i;

	h = gethostbyname(hostname);

	if (h == NULL) {
		return -1;
	}

	in_addr_list = (struct in_addr **) h->h_addr_list;

	for(i = 0; in_addr_list[i] != NULL; i++) {
		break;
	}

	asprintf(ip, "%s", inet_ntoa(*in_addr_list[i]));
	return 0;
}

int remote_connect(int *fd, const char *remote_ip, int port)
{
	struct sockaddr_in server;

	server.sin_addr.s_addr = inet_addr(remote_ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(*fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
		return -1;
	}

	return 0;
}

int remote_send()
{
	return 0;
}

int remote_receive()
{
	return 0;
}
