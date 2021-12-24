#include "lib/raylib/src/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_STATIC
#include "include/client.h"
#include "include/main.h"
#include "include/server.h"
#include "lib/raylib/src/extras/raygui.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int game_running;
extern Rectangle game[3][3];

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
	while (!game_running && !WindowShouldClose()) {
		char *clipboard = malloc(16);
		memcpy(clipboard, GetClipboardText(), 16);
		BeginDrawing();
		if (selection_step == 0) { // starting selection
			DrawText("Select Game Mode", (SCR_WIDTH - MeasureText("Select Game Mode", 20)) / 2, (SCR_HEIGHT / 2) - 100, 20, DARKGRAY);
			if (GuiButton((Rectangle){10, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, 50}, "Single Player")) {
				selection_step++;
				game_mode = 1;
			} else if (GuiButton((Rectangle){(SCR_WIDTH / 2) + 5, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, 50}, "Multi Player")) {
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
			DrawText("Select Game Hosting", (SCR_WIDTH - MeasureText("Select Game Hosting", 20)) / 2, (SCR_HEIGHT / 2) - 100, 20, DARKGRAY);
			if (GuiButton((Rectangle){10, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, 50}, "Host")) {
				selection_step++;
				game_hosting = 1;
			} else if (GuiButton((Rectangle){(SCR_WIDTH / 2) + 5, (SCR_HEIGHT / 2) - 60, (SCR_WIDTH / 2) - 15, 50}, "Join")) {
				selection_step += 2;
				game_hosting = 2;
			}
		} else if (selection_step == 2 && game_hosting == 1) // hosting multi player
		{
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int portbox_selected = CheckCollisionPointRec(GetMousePosition(), portBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && portbox_selected)
				sprintf(portchar, "%s", clipboard);
			if (GuiTextBox(nickBox, data->username, 20, nickbox_selected) || GuiTextBox(portBox, portchar, 20, portbox_selected)) {
				*PORT = atoi(portchar);
				sprintf(IP_ADDRESS, "127.0.0.1");
				game_running = 1;
				ret			 = 1;
			}
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - 35, 20, DARKGRAY);
			DrawText("Port:", 10, (SCR_HEIGHT / 3) + 5, 20, DARKGRAY);
		} else if (selection_step == 3) // join multi player
		{
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int ipbox_selected	 = CheckCollisionPointRec(GetMousePosition(), ipBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->username, "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && ipbox_selected)
				sprintf(IP_ADDRESS, "%s", clipboard);
			if (GuiTextBox(nickBox, data->username, 20, nickbox_selected) || GuiTextBox(ipBox, IP_ADDRESS, 16, ipbox_selected)) {
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
						DrawText(TextFormat("Error: connection to %s\non port %i failed!", IP_ADDRESS, 5555), 10, 10, 20, RED);
						ClearBackground(YELLOW);
						EndDrawing();
					}
					sprintf(IP_ADDRESS, " ");
				}
			}
			DrawText("Nickname:", 10, (SCR_HEIGHT / 3) - 35, 20, DARKGRAY);
			DrawText("IP:", 10, (SCR_HEIGHT / 3) + 5, 20, DARKGRAY);
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	return ret;
}

void matchInfo(struct client_data *data) { // draw match info
	const char *info_text = TextFormat("It's %s %s turn!", data->turn ? data->users[0] : data->users[1], data->turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, 20)) / 2, BLOCK * 3 + 10, 20, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", data->users[0], data->winsP[0], data->users[1], data->winsP[1]), 10, BLOCK * 3 + 40, 20, BLACK);
}