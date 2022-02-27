#include "../lib/raylib/src/raylib.h"
#ifndef __ANDROID_API__
	#define RAYGUI_IMPLEMENTATION
	#define RAYGUI_SUPPORT_ICONS
	#define RAYGUI_STATIC
	#include "../lib/raylib/src/extras/raygui.h"
#endif
#include "include/client.h"
#include "include/gui.h"
#include "include/main.h"
#include "include/server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int game_running;
extern Rectangle game[3][3];

#ifdef __ANDROID_API__
	#include <android_native_app_glue.h>
	#include <jni.h>
void toggleKeyboard(bool show) {
	// some "aliases"
	JavaVM *lJavaVM = app->activity->vm;
	JNIEnv *lJNIEnv = app->activity->env;

	// java vm args
	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name	  = "NativeThread";
	lJavaVMAttachArgs.group	  = NULL;

	// attaching to the java vm
	if ((*lJavaVM)->AttachCurrentThread(lJavaVM, &lJNIEnv, &lJavaVMAttachArgs) == JNI_ERR) return;

	// get native activity class and the method that we need
	jobject lNativeActivity	   = app->activity->clazz;
	jclass ClassNativeActivity = (*lJNIEnv)->GetObjectClass(lJNIEnv, lNativeActivity);
	// method name depending on the show parameter
	char *lMethodName = NULL;
	if (show)
		lMethodName = "showKeyboard";
	else
		lMethodName = "hideKeyboard";
	jmethodID _method = (*lJNIEnv)->GetMethodID(lJNIEnv, ClassNativeActivity, lMethodName, "()V");

	// call that method
	(*lJNIEnv)->CallVoidMethod(lJNIEnv, lNativeActivity, _method);
}

// some raygui reimplementations that did not work for android
int SCR_HEIGHT = 0;
int SCR_WIDTH  = 0;
Vector2 get_touch_pos() {
	Vector2 touch_pos = GetMousePosition();
	touch_pos.x *= SCR_WIDTH;
	touch_pos.y *= SCR_HEIGHT;
	return touch_pos;
}

bool GuiButton(Rectangle bounds, const char *text) {
	bool pressed	  = false;
	bool selected	  = false;
	Vector2 mouse_pos = get_touch_pos();
	selected		  = CheckCollisionPointRec(mouse_pos, bounds);
	pressed			  = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
	DrawRectangleRec(bounds, LIGHTGRAY);
	DrawRectangleLinesEx(bounds, 3, DARKGRAY);
	DrawText(text, bounds.x + ((bounds.width - MeasureText(text, STTT_TEXT_SIZE)) / 2), bounds.y + ((bounds.height - STTT_TEXT_SIZE) / 2), STTT_TEXT_SIZE, BLACK);
	DrawRectangleRec(bounds, Fade(BLUE, 0.3f * selected));
	return pressed && selected;
}

bool is_shift_pressed = false;
bool GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode) {
	DrawRectangleRec(bounds, RAYWHITE);
	DrawRectangleLinesEx(bounds, 3, LIGHTGRAY);
	int current_key_press = 0;
	if (editMode) {
		current_key_press = GetKeyPressed();
		DrawRectangleRec(bounds, Fade(BLUE, 0.3f));
		if (current_key_press != 0) {
			int txt_len = strlen(text);
			if (current_key_press == 67 && txt_len > 0)
				text[txt_len - 1] = '\0';
			if (txt_len < textSize) {
				if (current_key_press >= 7 && current_key_press <= 16) { // numbers
					text[txt_len]	  = current_key_press + 41;
					text[txt_len + 1] = '\0';
				} else { // letters
					if (current_key_press == 59)
						is_shift_pressed = true;
					else {
						text[txt_len] = current_key_press +
										(68 * !is_shift_pressed) +		   // not shift
										(36 * is_shift_pressed) +		   // shift
										(-78 * (current_key_press == 56)); // . character
						text[txt_len + 1] = '\0';
						is_shift_pressed  = false;
					}
				}
			}
		}
	}
	DrawText(text, bounds.x + 10, bounds.y + 2, textSize, BLACK);
	return current_key_press == 66;
}
bool GuiToggle(Rectangle bounds, const char *text, bool active) {
	Vector2 mouse_pos = get_touch_pos();
	if (CheckCollisionPointRec(mouse_pos, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		active = !active;
	DrawRectangleRec(bounds, LIGHTGRAY);
	DrawRectangleLinesEx(bounds, 3, DARKGRAY);
	DrawText(text, bounds.x + ((bounds.width - MeasureText(text, STTT_TEXT_SIZE)) / 2), bounds.y + ((bounds.height - STTT_TEXT_SIZE) / 2), STTT_TEXT_SIZE, BLACK);
	DrawRectangleRec(bounds, Fade(BLUE, 0.3f * active));
	return active;
}
#else
int SCR_WIDTH  = 450;
int SCR_HEIGHT = 800;
#endif

void initHitBox() { // creating boxes to detect touch or mouse clicks
	for (int i = 0; i < 9; i++) {
		game[i / 3][i % 3].x	  = (BLOCK * (i % 3)) + THICKNESS;
		game[i / 3][i % 3].y	  = (BLOCK * (i / 3)) + THICKNESS;
		game[i / 3][i % 3].height = BLOCK - 1;
		game[i / 3][i % 3].width  = BLOCK - 1;
	}
}

void grid() {
	// Borders
	DrawLineEx((Vector2){0, 1}, (Vector2){SCR_WIDTH, 1}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){1, 1}, (Vector2){1, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, SCR_WIDTH}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){SCR_WIDTH, 0}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// vertical grid
	DrawLineEx((Vector2){BLOCK, 0}, (Vector2){BLOCK, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){BLOCK * 2, 0}, (Vector2){BLOCK * 2, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// horizontal grid
	DrawLineEx((Vector2){0, BLOCK}, (Vector2){SCR_WIDTH, BLOCK}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, BLOCK * 2}, (Vector2){SCR_WIDTH, BLOCK * 2}, THICKNESS + 1, BLACK);
}

int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data) {
	int ret = -1, selection_step = 0, game_mode = -1, game_hosting = -1;
	char portchar[16] = "5555";
	Rectangle nickBox = {MeasureText("Nickname:", STTT_TEXT_SIZE) + 15, (SCR_HEIGHT / 3) - (STTT_TEXT_SIZE * 2) - 3, SCR_WIDTH - MeasureText("Nickname:", STTT_TEXT_SIZE) - 25, STTT_TEXT_SIZE * 1.3};
	Rectangle ipBox	  = {MeasureText("IP:", STTT_TEXT_SIZE) + 15, SCR_HEIGHT / 3, SCR_WIDTH - MeasureText("IP:", STTT_TEXT_SIZE) - 25, STTT_TEXT_SIZE * 1.3};
	Rectangle portBox = {MeasureText("Port:", STTT_TEXT_SIZE) + 15, SCR_HEIGHT / 3, SCR_WIDTH - MeasureText("Port:", STTT_TEXT_SIZE) - 25, STTT_TEXT_SIZE * 1.3};
	char *clipboard	  = malloc(17);

	bool nickbox_selected = false;
	bool ipbox_selected	  = false;
	bool portbox_selected = false;

#ifdef __ANDROID_API__
	bool is_keyboard_open = false;
#endif
	while (!game_running && !WindowShouldClose()) {
#ifdef __ANDROID_API__
		if ((nickbox_selected || portbox_selected || ipbox_selected) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			toggleKeyboard(is_keyboard_open = true);
#else
		const char *rawclipboard = GetClipboardText();
		if (rawclipboard)
			memcpy(clipboard, rawclipboard, 16);
#endif
		BeginDrawing();
		if (selection_step == 0) { // starting selection
			DrawText("Select Game Mode", (SCR_WIDTH - MeasureText("Select Game Mode", STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - (SCR_HEIGHT / 9) - 20, STTT_TEXT_SIZE, DARKGRAY);
			if (GuiButton((Rectangle){10, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Single Player")) {
				selection_step++;
				game_mode = 1;
			} else if (GuiButton((Rectangle){(SCR_WIDTH / 2) + 5, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Multi Player")) {
				selection_step++;
				game_mode = 2;
			}
		} else if (selection_step == 1 && game_mode == 1) // single player
		{
			*PORT = 5555;
			sprintf(IP_ADDRESS, "127.0.0.1");
			game_running = 1;
			ret			 = 2;
		} else if (selection_step == 1 && game_mode == 2) // multi player
		{
			DrawText("Select Game Hosting", (SCR_WIDTH - MeasureText("Select Game Hosting", STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - (SCR_HEIGHT / 9) - 20, STTT_TEXT_SIZE, DARKGRAY);
			if (GuiButton((Rectangle){10, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Host")) {
				selection_step++;
				game_hosting = 1;
			} else if (GuiButton((Rectangle){(SCR_WIDTH / 2) + 5, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Join")) {
				selection_step += 2;
				game_hosting = 2;
			}
		} else if (selection_step == 2 && game_hosting == 1) // hosting multi player
		{
#ifdef __ANDROID_API__
			nickbox_selected = CheckCollisionPointRec(get_touch_pos(), nickBox) || !portbox_selected;
			portbox_selected = CheckCollisionPointRec(get_touch_pos(), portBox) || !nickbox_selected;
#else
			nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox) || !portbox_selected;
			portbox_selected = CheckCollisionPointRec(GetMousePosition(), portBox) || !nickbox_selected;
#endif
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && portbox_selected)
				sprintf(portchar, "%s", clipboard);
			bool enter_pressed = GuiTextBox(nickBox, data->username, STTT_TEXT_SIZE, nickbox_selected);
			enter_pressed |= GuiTextBox(portBox, portchar, STTT_TEXT_SIZE, portbox_selected);
#ifdef __ANDROID_API__
			enter_pressed |= GuiButton((Rectangle){(SCR_WIDTH - 30) / 4, (SCR_HEIGHT / 2) - 140, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Host");
#endif
			if (enter_pressed) {
				*PORT = atoi(portchar);
				sprintf(IP_ADDRESS, "127.0.0.1");
				game_running = 1;
				ret			 = 1;
			}
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - (STTT_TEXT_SIZE * 2), STTT_TEXT_SIZE, DARKGRAY);
			DrawText("Port:", 10, (SCR_HEIGHT / 3) + (STTT_TEXT_SIZE / 6), STTT_TEXT_SIZE, DARKGRAY);
		} else if (selection_step == 3) // join multi player
		{
#ifdef __ANDROID_API__
			nickbox_selected = CheckCollisionPointRec(get_touch_pos(), nickBox) || !ipbox_selected;
			ipbox_selected	 = CheckCollisionPointRec(get_touch_pos(), ipBox) || !nickbox_selected;
#else
			nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox) || !ipbox_selected;
			ipbox_selected	 = CheckCollisionPointRec(GetMousePosition(), ipBox) || !nickbox_selected;
#endif
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && ipbox_selected)
				sprintf(IP_ADDRESS, "%s", clipboard);
			bool enter_pressed = GuiTextBox(nickBox, data->username, STTT_TEXT_SIZE, nickbox_selected);
			enter_pressed |= GuiTextBox(ipBox, IP_ADDRESS, STTT_TEXT_SIZE, ipbox_selected);
#ifdef __ANDROID_API__
			enter_pressed |= GuiButton((Rectangle){(SCR_WIDTH - 30) / 4, (SCR_HEIGHT / 2) - 140, (SCR_WIDTH / 2) - 15, (SCR_HEIGHT / 18)}, "Connect");
#endif
			if (enter_pressed) {
				if (strlen(IP_ADDRESS) <= 1)
					sprintf(IP_ADDRESS, "127.0.0.1");
				if (client_connect(IP_ADDRESS, 5555, &data->sockfd) != -1) {
					game_running = 1;
					ret			 = 0;
				} else {
					int tmp_time = time(NULL) + 3;
					ret			 = -1;
					while (time(NULL) != tmp_time) {
						BeginDrawing();
						DrawText(TextFormat("Error: connection to %s\non port %i failed!", IP_ADDRESS, 5555), 10, 10, STTT_TEXT_SIZE, RED);
						ClearBackground(YELLOW);
						EndDrawing();
					}
					sprintf(IP_ADDRESS, " ");
				}
			}
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - (STTT_TEXT_SIZE * 2), STTT_TEXT_SIZE, DARKGRAY);
			DrawText("IP:", 10, (SCR_HEIGHT / 3) + 5, STTT_TEXT_SIZE, DARKGRAY);
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	return ret;
}

void matchInfo(struct client_data *data) { // draw match info
	const char *info_text = TextFormat("It's %s %s turn!", data->turn ? data->users[0] : data->users[1], data->turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, STTT_TEXT_SIZE)) / 2, BLOCK * 3 + 10, STTT_TEXT_SIZE, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", data->users[0], data->winsP[0], data->users[1], data->winsP[1]), 10, BLOCK * 3 + 40, STTT_TEXT_SIZE, BLACK);
	DrawText(TextFormat("%s\n", data->local_ip), 10, SCR_HEIGHT - STTT_TEXT_SIZE * 2, STTT_TEXT_SIZE, BLACK);
}