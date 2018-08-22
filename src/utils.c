/**
 * @file   utils.c
 * @brief  utility functions
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "utils.h"

int push_list(struct iflist **ifl, char *name, char *ip, char *mac)
{
	struct iflist *new = (struct iflist *) malloc(sizeof(struct iflist));
	new->next = (*ifl);

	memcpy(new->name, name, IFNAMSIZ);
	memcpy(new->ip, ip, INET_ADDRSTRLEN);
	memcpy(new->mac, mac, 19);

	(*ifl) = new;
	return 0;
}

int dump_list(struct iflist *ifl)
{
	if (ifl == NULL) {
		return -1;
	}

	while (ifl != NULL) {
		printf("if: %8s\t ip: %16s\tmac: %s\n",
			ifl->name, ifl->ip, ifl->mac);
		ifl = ifl->next;
	}
	return 0;
}

int jsonify_interface(char **json, char *ifname, char *ip, char *mac)
{
	if (ifname == NULL || ip == NULL || mac == NULL) {
		return -1;
	}

	asprintf(json, "{\"name\": \"%s\", \"ip\": \"%s\", \"mac\": \"%s\"}", ifname, ip, mac);
	return 0;
}

int jsonify(char **json, struct iflist *ifl, char *hostname, char *ext_ip)
{
	char *tmp = NULL;
	if (ifl == NULL || hostname == NULL || ext_ip == NULL) {
		return -1;
	}

	while (ifl != NULL) {
		jsonify_interface(&tmp,ifl->name, ifl->ip, ifl->mac);
		if (tmp == NULL)
			return -2;

		if (*json == NULL)
			asprintf(json, "{\"interfaces\": [%s", tmp);
		else 
			asprintf(json, "%s,%s", *json, tmp);
		free(tmp);
		tmp = NULL;
		ifl = ifl->next;
	}
	asprintf(json, "%s],\"hostname\": \"%s\",\"external_ip\": \"%s\"}", *json, hostname, ext_ip);
	free(tmp);
	tmp = NULL;
	return 0;
}

