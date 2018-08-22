/**
 * @file   utils.h
 * @brief  utility definitions
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */

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

int push_list(struct iflist **, char *, char *, char *);
int dump_list(struct iflist *);
int jsonify_interface(char **, char *, char *, char *);
int jsonify_list(char **, struct iflist *);

#endif /* __UTILS_H */
