#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
void initHitBox();
void grid();
int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data);
void matchInfo(struct client_data *data);

#ifdef __ANDROID_API__
	#define STTT_TEXT_SIZE 50
	#include "../lib/raylib/src/raylib.h"
Vector2 get_touch_pos();
bool GuiButton(Rectangle bounds, const char *text);
bool GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode);
bool GuiToggle(Rectangle bounds, const char *text, bool active);
#else
	#define STTT_TEXT_SIZE 20
#endif

#endif // __GUI_H__