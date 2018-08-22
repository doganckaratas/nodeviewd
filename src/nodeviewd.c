/** 
 * @file   nodeviewd.c
 * @brief  node view daemon
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <string.h> //delete

#include "socket.h"
#include "utils.h"

int main() {
	int fd = 0;
	struct ifconf ifc;
	struct ifreq ifr;
	struct iflist *ifl = NULL;
	char *json_interfaces = NULL;

	if (open_socket(&fd) < 0) {
		fprintf(stderr, "Unable to open file descriptor.\n");
		return -1;
	}

	if (get_ifaces(&fd, &ifc) < 0) {
		fprintf(stderr, "Unable to get interfaces.\n");
		return -2;
	}

	if (get_if_data(&fd, &ifc, &ifr, &ifl) < 0) {
		fprintf(stderr, "Unable to fetch interface data.\n");
		return -3;
	}
	
	if (jsonify_list(&json_interfaces, ifl) < 0) {
		fprintf(stderr, "Unable to dump interface data.\n");
		return -4;
	}

	printf("json array: %s\n", json_interfaces);

	if (close_socket(&fd) < 0) {
		fprintf(stderr, "Unable to close file descriptor.\n");
		return -1;
	}

	if (ifl != NULL) {
		free(ifl);
		ifl = NULL;
	}

	if (json_interfaces == NULL) {
		free(json_interfaces);
		json_interfaces = NULL;
	}
	return 0;
}

