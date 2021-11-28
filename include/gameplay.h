#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "main.h"

void end_client_game(struct client_data *data);
int checkwinner(struct client_data *data);
void end_server_game(int winner, struct client_data *data);

#endif // __GAMEPLAY_H__