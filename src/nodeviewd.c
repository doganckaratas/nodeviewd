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
#include "remote.h"
#include "utils.h"

#ifdef DEBUG
#define log_d(...)	fprintf(stdout, __VA_ARGS__);
#define log_e(...)	fprintf(stderr, __VA_ARGS__);
#else
#define log_d(...)
#define log_d(...)
#endif /* DEBUG */

int main() {
	int fd_ioctl = 0, fd_remote = 0;
	struct ifconf ifc;
	struct ifreq ifr;
	struct iflist *ifl = NULL;
	/* we need to config file for these parameters. */
	/* also if there is no config file, these values will be default */
	char *remotehost = "nodeview.herokuapp.com";
	char responseview[1024] = {0};
	char *json = NULL;
	char *remoteip = NULL;
	int remoteport = 80;

	log_d("nodeviewd running...\n");

	if (open_socket(&fd_ioctl, IOCTL) < 0) {
		log_d("Unable to open file descriptor.\n");
		return -1;
	}

	if (open_socket(&fd_remote, STREAM) < 0) {
		log_d("Unable to open file descriptor.\n");
		return -1;
	}

	if (get_ifaces(&fd_ioctl, &ifc) < 0) {
		log_d("Unable to get interfaces.\n");
		return -2;
	}

	if (get_if_data(&fd_ioctl, &ifc, &ifr, &ifl) < 0) {
		log_d("Unable to fetch interface data.\n");
		return -3;
	}

	if (jsonify(&json, ifl, "ubuntu", "172.23.32.22") < 0) {
		log_d("Unable to JSONify interface data.\n");
		return -4;
	}

	if (remote_resolve_ip(remotehost, &remoteip) < 0) {
		log_d("Unable to resolve host %s.\n", remotehost);
		return -5;
	}
	log_d("Connection parameters:\nRemote host: %s\nRemote ip: %s\nRemote port: %d\n", remotehost, remoteip, remoteport);
	log_d("Generated JSON:\n%s\n", json);

	if (remote_connect(&fd_remote, remoteip, remoteport) < 0) {
		log_d("Unable to connect %s (%s:%d).\n", remotehost, remoteip, remoteport);
		return -5;
	}

	log_d("Connected to %s:%d\n", remotehost, remoteport);

	if (remote_send(&fd_remote, GET, "{\"hostname\":\"home\"}", responseview) < 0) {
		log_d("Unable to request data to server, %s:%d\n", remotehost, remoteport);
		return -5;
	}

	printf("Server response: \n%s\n", responseview);

	if (close_socket(&fd_ioctl) < 0) {
		log_d("Unable to close file descriptor.\n");
		return -1;
	}

	if (close_socket(&fd_remote) < 0) {
		log_d("Unable to close file descriptor.\n");
		return -1;
	}

	if (ifl != NULL) {
		free(ifl);
		ifl = NULL;
	}

	if (json != NULL) {
		free(json);
		json = NULL;
	}

	if (remoteip != NULL) {
		free(remoteip);
		remoteip = NULL;
	}
	return 0;
}

