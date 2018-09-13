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
#include "utils.h"

#define REQUEST_BODY "%s /machine/%s/ HTTP/1.1\r\nHost: nodeview.herokuapp.com\r\n"
#define CONTENT_DATA "Content-Type: application/json\r\nContent-Length: %lu\r\n\r\n%s"

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

int remote_send(int *fd, enum http_action action, char * payload, char *response)
{
	/* FIXME: receive until no data left. */
	if (fd == NULL) {
		return -1;
	}

	char *request = NULL;

	switch (action) {
		case GET:
			asprintf(&request, REQUEST_BODY "\r\n", "GET", "view");
			break;
		case POST:
			asprintf(&request, REQUEST_BODY CONTENT_DATA, "POST", "add", strlen(payload), payload);
			break;
		case DELETE:
			asprintf(&request, REQUEST_BODY CONTENT_DATA, "DELETE", "delete", strlen(payload), payload);
			break;
		default:
			return -2;
	}

	log_d("Request: \n%s\n", request);

	if (send(*fd, request, strlen(request), 0) < 0) {
		return -2;
	}

	if (recv(*fd, response, 1024, 0) < 0) {
		return -3;
	}

	return 0;
}
#if 0
int remote_receive(int *fd, char *response)
{
	if (fd == NULL || response == NULL) {
		return -1;
	}
	/* FIXME: receive until no data left. */

	char *data = malloc(1024);
	if (recv(*fd, data, strlen(data), 0) < 0) {
		return -2;
	}

	return 0;
}
#endif
