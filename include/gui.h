#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
void initHitBox();
void grid();
int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data);
void matchInfo(struct client_data *data);
#ifdef ANDROID
	#define STTT_TEXT_SIZE 40
	#include "../lib/raylib/src/raylib.h"
Vector2 get_touch_pos();
bool gui_button(Rectangle bounds, const char *text);
bool gui_text_box(Rectangle bounds, char *text, int textSize, bool editMode);
bool gui_toggle(Rectangle bounds, const char *text, bool active);
	#define GetMousePosition() get_touch_pos()
	#define GuiButton(x, y) gui_button(x, y)
	#define GuiTextBox(x, y, z, w) gui_text_box(x, y, z, w)
	#define GuiToggle(x, y, z) gui_toggle(x, y, z)
#else
	#define STTT_TEXT_SIZE 20
#endif

#endif // __GUI_H__