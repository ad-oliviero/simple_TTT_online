#ifndef __CLIENT_H__
#define __CLIENT_H__

int client_connect(char *IP_ADDRESS, int PORT);
void *client_comm();

#endif // __CLIENT_H__