#ifndef __UTILS_H
#define __UTILS_H

#include <arpa/inet.h>
#include <net/if.h>

struct iflist {
	char name[IFNAMSIZ];
	char ip[INET_ADDRSTRLEN];
	char mac[19];
	struct iflist *next;
};

int push(struct iflist **, char *, char *, char *);
int dump(struct iflist *);

#endif /* __UTILS_H */
