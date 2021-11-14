#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "main.h"

void end_client_game(struct online_data *data);
int checkwinner();
void end_server_game(int winner);

#endif // __GAMEPLAY_H__