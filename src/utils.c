#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "utils.h"

int push(struct iflist **head, char *name, char *ip, char *mac)
{
	struct iflist *new = (struct iflist *) malloc(sizeof(struct iflist));
	new->next = (*head);

	memcpy(new->name, name, IFNAMSIZ);
	memcpy(new->ip, ip, INET_ADDRSTRLEN);
	memcpy(new->mac, mac, 19);

	(*head) = new;
	return 0;
}

int dump(struct iflist *ifl)
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
