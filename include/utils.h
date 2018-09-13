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

#define VERSION		"v0.1"
#if defined(DEBUG)
#define log_d(...)	fprintf(stdout, "\33[33;1m[DEBUG]\33[0m " __VA_ARGS__);
#define log_i(...)	fprintf(stdout, "\33[32;1m[ INFO]\33[0m " __VA_ARGS__);
#define log_e(...)	fprintf(stderr, "\33[31;1m[ERROR]\33[0m " __VA_ARGS__);
#elif defined(INFO)
#define log_d(...)
#define log_i(...)	fprintf(stdout, "\33[32;1m[ INFO]\33[0m " __VA_ARGS__);
#define log_e(...)	fprintf(stderr, "\33[31;1m[ERROR]\33[0m " __VA_ARGS__);
#elif defined(ERROR)
#define log_d(...)
#define log_i(...)
#define log_e(...)	fprintf(stderr, "\33[31;1m[ERROR]\33[0m " __VA_ARGS__);
#else
#define log_d(...)
#define log_i(...)
#define log_e(...)
#endif

struct iflist {
	char name[IFNAMSIZ];
	char ip[INET_ADDRSTRLEN];
	char mac[19];
	struct iflist *next;
};

int push_list(struct iflist **, const char *, const char *, const char *);
int dump_list(struct iflist *);
int jsonify_interface(char **, const char *, const char *, const char *);
int jsonify(char **, struct iflist *, const char *, const char *);

#endif /* __UTILS_H */
