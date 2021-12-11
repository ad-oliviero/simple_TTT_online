#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
void initHitBox();
void grid();
int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data);
void matchInfo(struct client_data *data);

#endif // __GUI_H__