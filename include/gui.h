#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
void initHitBox();
void grid();
int join_window(char *IP_ADDRESS, int *PORT);
void matchInfo(struct online_data *data);

#endif // __GUI_H__