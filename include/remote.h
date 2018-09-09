/**
 * @file   remote.h
 * @brief  remote server definitions
 * @date   23/08/2018
 * @author Dogan Can Karatas
 */


#ifndef _REMOTE_H
#define _REMOTE_H

enum http_action {
	GET,
	POST,
	DELETE
};

int remote_resolve_ip(const char *, char **);
int remote_connect(int *, const char*, int);
int remote_send(int *, enum http_action, char *, char *);


#endif /* _REMOTE_H */
