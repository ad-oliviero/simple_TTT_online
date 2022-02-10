#include "../lib/raylib/src/raylib.h"
// #ifndef __ANDROID_API__
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_STATIC
#include "../lib/raylib/src/extras/raygui.h"
// #endif
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
int SCR_WIDTH  = 1080;
int SCR_HEIGHT = 1920;
/* int SCR_HEIGHT = 0;
int SCR_WIDTH  = 0;
Vector2 __wrap_GetMousePosition() {
	Vector2 touch_pos = GetMousePosition();
	touch_pos.x *= SCR_WIDTH;
	touch_pos.y *= SCR_HEIGHT;
	return touch_pos;
} */

/* bool GuiButton(Rectangle bounds, const char *text) {
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
} */

// bool GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode) { return false; }

// bool GuiToggle(Rectangle bounds, const char *text, bool active) { return true; }
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
	Rectangle nickBox = {MeasureText("Nickname:", 20) + 15, (SCR_HEIGHT / 3) - 40, SCR_WIDTH - MeasureText("Nickname:", 20) - 25, 30};
	Rectangle ipBox	  = {MeasureText("IP:", 20) + 15, SCR_HEIGHT / 3, SCR_WIDTH - MeasureText("IP:", 20) - 25, 30};
	Rectangle portBox = {MeasureText("Port:", 20) + 15, SCR_HEIGHT / 3, SCR_WIDTH - MeasureText("Port:", 20) - 25, 30};
	char *clipboard	  = malloc(17);
	while (!game_running && !WindowShouldClose()) {
#ifndef __ANDROID_API__
		const char *rawclipboard = GetClipboardText();
		if (rawclipboard)
			memcpy(clipboard, rawclipboard, 16);
#endif
		BeginDrawing();
		// DrawText(TextFormat("%d\n%d", GetMonitorHeight(0), GetMonitorWidth(0)), 20, 60, 40, BLACK);
		if (selection_step == 0) { // starting selection
			DrawText("Select Game Mode", (SCR_WIDTH - MeasureText("Select Game Mode", STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - (SCR_HEIGHT / 9) - 15, STTT_TEXT_SIZE, DARKGRAY);
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
			DrawText("Select Game Hosting", (SCR_WIDTH - MeasureText("Select Game Hosting", STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - 100, STTT_TEXT_SIZE, DARKGRAY);
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
			sprintf(data->username, "Android");
			*PORT = atoi(portchar);
			sprintf(IP_ADDRESS, "127.0.0.1");
			game_running = 1;
			ret			 = 1;
#else
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int portbox_selected = CheckCollisionPointRec(GetMousePosition(), portBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && portbox_selected)
				sprintf(portchar, "%s", clipboard);
			if (GuiTextBox(nickBox, data->username, STTT_TEXT_SIZE, nickbox_selected) || GuiTextBox(portBox, portchar, STTT_TEXT_SIZE, portbox_selected)) {
				*PORT = atoi(portchar);
				sprintf(IP_ADDRESS, "127.0.0.1");
				game_running = 1;
				ret			 = 1;
			}
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - 35, STTT_TEXT_SIZE, DARKGRAY);
			DrawText("Port:", 10, (SCR_HEIGHT / 3) + 5, STTT_TEXT_SIZE, DARKGRAY);
#endif
		} else if (selection_step == 3) // join multi player
		{
#ifdef __ANDROID_API__
			sprintf(IP_ADDRESS, "192.168.1.222");
			sprintf(data->username, "Android");
			if (client_connect(IP_ADDRESS, 5555, &data->sockfd) != -1) {
				game_running = 1;
				ret			 = 0;
			}
#else
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int ipbox_selected	 = CheckCollisionPointRec(GetMousePosition(), ipBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && ipbox_selected)
				sprintf(IP_ADDRESS, "%s", clipboard);
			if (GuiTextBox(nickBox, data->username, STTT_TEXT_SIZE, nickbox_selected) || GuiTextBox(ipBox, IP_ADDRESS, 16, ipbox_selected)) {
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
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - 35, STTT_TEXT_SIZE, DARKGRAY);
			DrawText("IP:", 10, (SCR_HEIGHT / 3) + 5, STTT_TEXT_SIZE, DARKGRAY);
#endif
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
}