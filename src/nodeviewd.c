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

int main(int argc, char **argv) {
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

	log_i("nodeviewd " VERSION " running...\n\n");

	if (open_socket(&fd_ioctl, IOCTL) < 0) {
		log_e("Unable to open file descriptor.\n");
		return -1;
	}

	if (open_socket(&fd_remote, STREAM) < 0) {
		log_e("Unable to open file descriptor.\n");
		return -1;
	}

	if (get_ifaces(&fd_ioctl, &ifc) < 0) {
		log_e("Unable to get interfaces.\n");
		return -2;
	}

	if (get_if_data(&fd_ioctl, &ifc, &ifr, &ifl) < 0) {
		log_e("Unable to fetch interface data.\n");
		return -3;
	}

	if (jsonify(&json, ifl, "ubuntu", "172.23.32.22") < 0) {
		log_e("Unable to JSONify interface data.\n");
		return -4;
	}

	if (remote_resolve_ip(remotehost, &remoteip) < 0) {
		log_e("Unable to resolve host %s.\n", remotehost);
		return -5;
	}

	log_d("Connection parameters:\nRemote host: %s\nRemote ip: %s\nRemote port: %d\n", remotehost, remoteip, remoteport);
	log_d("Generated JSON:\n%s\n", json);

	if (remote_connect(&fd_remote, remoteip, remoteport) < 0) {
		log_e("Unable to connect %s (%s:%d).\n", remotehost, remoteip, remoteport);
		return -5;
	}

	log_d("Connected to %s:%d\n", remotehost, remoteport);

	/* INFO: get: -g, add -a, delete -d */
	if (argv[1] != NULL && argc == 2) {
		if (strcmp(argv[1], "-g") == 0) {
			if (remote_send(&fd_remote, GET, "", responseview) < 0) {
				log_e("Unable to request data to server, %s:%d\n", remotehost, remoteport);
				return -5;
			}
		} else if (strcmp(argv[1], "-a") == 0) {
			if (remote_send(&fd_remote, POST, json, responseview) < 0) {
				log_e("Unable to request data to server, %s:%d\n", remotehost, remoteport);
				return -5;
			}
		} else if (strcmp(argv[1], "-d") == 0) {
			if (remote_send(&fd_remote, DELETE, "{\"hostname\":\"ubuntu\"}", responseview) < 0) {
				log_e("Unable to request data to server, %s:%d\n", remotehost, remoteport);
				return -5;
			}
		} else {
			log_e("Unknown option %s.\n", argv[1]);
			return -5;
		}
	} else {
		/* Daemonize process if this case selected */
		log_e("Please specify an option.\n");
		return -5;
	}

	log_d("Response: \n%s\n", responseview);

	if (close_socket(&fd_ioctl) < 0) {
		log_e("Unable to close file descriptor.\n");
		return -1;
	}

	if (close_socket(&fd_remote) < 0) {
		log_e("Unable to close file descriptor.\n");
		return -1;
	}

	log_d("Cleaning...\n");
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

	log_d("Exiting...\n");
	return 0;
}

