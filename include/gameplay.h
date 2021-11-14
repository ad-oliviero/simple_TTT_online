#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "main.h"

void end_client_game(struct online_data *data);
int checkwinner(struct online_data *data);
void end_server_game(int winner, struct online_data *data);

#endif // __GAMEPLAY_H__