#ifndef __GUI_H__
#define __GUI_H__

#include "../../lib/raylib/src/raylib.h"
#include "main.h"

extern Color BG_COLOR;
extern Color FG_COLOR;

void initHitBox();
void grid();
int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data);
void matchInfo(struct client_data *data);

#ifdef ANDROID
	#define STTT_TEXT_SIZE 64
void toggleKeyboard(bool show);
#else
	#define STTT_TEXT_SIZE 20
#endif

#endif // __GUI_H__