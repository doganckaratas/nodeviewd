/**
 * @file   socket.h
 * @brief  socket definitions
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */


#ifndef _SOCKET_H
#define _SOCKET_H

#include "utils.h"

#define MAC_FMT		"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_STR(x)	(unsigned char) x[0], \
			(unsigned char) x[1], \
			(unsigned char) x[2], \
			(unsigned char) x[3], \
			(unsigned char) x[4], \
			(unsigned char) x[5]

enum socket_type {
	IOCTL=1,
	STREAM
};

int open_socket(int *, enum socket_type);
int close_socket(int *);
int get_ifaces(int *, struct ifconf *);
int get_if_data(int *, struct ifconf *, struct ifreq *, struct iflist **);

#endif /* _SOCKET_H */
