/** 
 * @file   socket.h
 * @brief  socket definitions 
 * @date   02/08/2018
 * @author Dogan Can Karatas
 */


#ifndef _SOCKET_H
#define _SOCKET_H

#include "utils.h"

int find_ifaces();
int open_socket(int *);
int close_socket(int *);
int get_ifaces(int *, struct ifconf *);
int get_if_data(int *, struct ifconf *, struct ifreq *, struct iflist **);

#endif /* _SOCKET_H */
