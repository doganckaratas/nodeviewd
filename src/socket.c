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
#include <linux/if_link.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "socket.h"

int find_ifaces()
{
	struct ifaddrs *addrs,*tmp;
	char host[NI_MAXHOST];
	int s = 0, n = 0;
	if (getifaddrs(&addrs) == -1) {
		perror ("getifaddrs");
		return -1;
	}

	for (tmp = addrs, n = 0; tmp != NULL; tmp = tmp->ifa_next, n++) {
		if (tmp->ifa_addr == NULL) {
			continue;
		}

		if (tmp->ifa_addr->sa_family == AF_INET) {
			s = getnameinfo(tmp->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return -2;
			}
			/* this line extracts per-node information. */
			/* http://man7.org/linux/man-pages/man3/getifaddrs.3.html */
			/* add these into linked-list and pass by reference */
			printf("%s : %s\n", tmp->ifa_name, host);
		}
	}
	freeifaddrs(addrs);
	return 0;
}
