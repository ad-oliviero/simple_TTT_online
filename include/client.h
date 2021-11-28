#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "main.h"

int client_connect(char *IP_ADDRESS, int PORT, SOCK *sock);
void *client_comm(void *arg);

#endif // __CLIENT_H__