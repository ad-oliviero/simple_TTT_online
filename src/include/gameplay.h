#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "../../lib/raylib-nuklear/include/raylib-nuklear.h"
#include "main.h"

void end_client_game(struct client_data *data, struct nk_context *ctx);
int checkwinner(struct client_data *data);
void end_server_game(int winner, struct client_data *data);

#endif // __GAMEPLAY_H__