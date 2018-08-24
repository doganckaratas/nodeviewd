/**
 * @file   socket.c
 * @brief  socket utilities
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_link.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "socket.h"
#include "utils.h"

int open_socket(int *fd, enum socket_type sock_e)
{
	if (fd == NULL) {
		return -1;
	}

	switch (sock_e) {
		case IOCTL:
			*fd = socket(PF_INET, SOCK_DGRAM, 0);
			break;
		case STREAM:
			*fd = socket(AF_INET, SOCK_STREAM, 0);
			break;
		default:
			return -2;
	}

	if (*fd < 0) {
		perror("socket");
		return -3;
	}

	return 0;
}

int close_socket(int *fd)
{
	if (fd == NULL) {
		return -1;
	}

	close(*fd);
	return 0;
}

int get_ifaces(int *fd, struct ifconf *ifc)
{
	char buf[8192] = {0};
	if (fd == NULL || ifc == NULL) {
		return -1;
	}

	ifc->ifc_len = sizeof(buf);
	ifc->ifc_buf = buf;
	if (ioctl(*fd, SIOCGIFCONF, ifc) < 0) {
		perror("ioctl(SIOCGIFCONF)");
		return -2;
	}

	return 0;
}

int get_if_data(int *fd, struct ifconf *ifc, struct ifreq *ifr, struct iflist **ifl)
{
	struct ifreq *item;
	struct sockaddr *addr;
	char ip[INET_ADDRSTRLEN] = {0};
	char mac[19] = {0};
	int intf_num = 0;

	ifr = ifc->ifc_req;
	intf_num = ifc->ifc_len / sizeof(struct ifreq);

	for (int i = 0; i < intf_num; i++) {
		item = (ifr + i);
		addr = &item->ifr_addr;
		if (ioctl(*fd, SIOCGIFADDR, item) < 0) {
			perror("ioctl(SIOCGIFADDR)");
			return -1;
		}

		if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip, sizeof(ip)) == NULL) {
			 perror("inet_ntop");
			 continue;
		}

		if (ioctl(*fd, SIOCGIFHWADDR, item) < 0) {
			perror("ioctl(SIOCGIFHWADDR)");
			return -2;
		}
		sprintf(mac, MAC_FMT, MAC_STR(item->ifr_hwaddr.sa_data));
		push_list(ifl, item->ifr_name, ip, mac);
	}
	return 0;
}
