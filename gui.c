#include "lib/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_STATIC
#include "lib/raygui/src/raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "include/main.h"
#include "include/client.h"
#include "include/server.h"

extern int block;
extern int game_running;
extern char user0[32];
extern Rectangle game[9];
extern pthread_t server_tid[128];

void initHitBox()
{ // creating boxes to detect touch or mouse clicks
	for (int i = 0; i < 3; i++)
	{
		game[i].x = i * block + 1;
		game[i].y = 1;
		// game[i].y = (block * (i / 3)) + 1;
		game[i].height = block;
		game[i].width = block;

		game[i + 3].x = i * block + 1;
		game[i + 3].y = block + 1;
		game[i + 3].height = block;
		game[i + 3].width = block;

		game[i + 6].x = i * block + 1;
		game[i + 6].y = block * 2 + 1;
		game[i + 6].height = block;
		game[i + 6].width = block;
	}
}

void grid()
{
	// Borders
	DrawLineEx((Vector2){0, 1}, (Vector2){SCR_WIDTH, 1}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){1, 1}, (Vector2){1, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, SCR_WIDTH}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){SCR_WIDTH, 0}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// vertical grid
	DrawLineEx((Vector2){block, 0}, (Vector2){block, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){block * 2, 0}, (Vector2){block * 2, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// horizontal grid
	DrawLineEx((Vector2){0, block}, (Vector2){SCR_WIDTH, block}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, block * 2}, (Vector2){SCR_WIDTH, block * 2}, THICKNESS + 1, BLACK);
}

int join_window(char *IP_ADDRESS, int *PORT, struct online_data *data)
{
	int ret = -1, selection_step = 0, game_mode = -1, game_hosting = -1;
	char portchar[16] = "5555";
	SetTraceLogLevel(LOG_NONE);
	InitWindow(320, 75, "Game Mode Selection");
	// InitWindow(320, 75, "Set nickname and IP address");
	SetTargetFPS(GetMonitorRefreshRate(0));
	Rectangle nickBox = {MeasureText("Nickname:", 20) + 15, 5, 200, 30};
	Rectangle ipBox = {MeasureText("IP:", 20) + 15, 40, 267, 30};
	Rectangle portBox = {MeasureText("Port:", 20) + 15, 40, 242, 30};
	while (!game_running && !WindowShouldClose())
	{
		char *clipboard = (char *)GetClipboardText();
		clipboard[17] = '\0';
		BeginDrawing();
		if (selection_step == 0) // starting selection
		{
			DrawText("Select Game Mode", (320 - MeasureText("Select Game Mode", 20)) / 2, 5, 20, DARKGRAY);
			if (GuiButton((Rectangle){5, 30, 150, 40}, "Single Player"))
			{
				int tmp_time = time(NULL) + 2;
				ret = -1;
				// selection_step++;
				while (time(NULL) != tmp_time)
				{
					BeginDrawing();
					DrawText(TextFormat("  Single Player mode\nis not yet implemented!", IP_ADDRESS, 5555), (320 - MeasureText(TextFormat("Single Player mode\nis not yet implemented!", IP_ADDRESS, 5555), 20)) / 2, 10, 20, ORANGE);
					ClearBackground(RAYWHITE);
					EndDrawing();
				}
				game_mode = 1;
			}
			else if (GuiButton((Rectangle){165, 30, 150, 40}, "Multi Player"))
			{
				selection_step++;
				game_mode = 2;
			}
		}
		else if (selection_step == 1 && game_mode == 1) // single player
		{
			// TOTO: single player mode
		}
		else if (selection_step == 1 && game_mode == 2) // multi player
		{
			DrawText("Select Game Hosting", (320 - MeasureText("Select Game Hosting", 20)) / 2, 5, 20, DARKGRAY);
			if (GuiButton((Rectangle){5, 30, 150, 40}, "Host"))
			{
				selection_step++;
				game_hosting = 1;
			}
			else if (GuiButton((Rectangle){165, 30, 150, 40}, "Join"))
			{
				selection_step += 2;
				game_hosting = 2;
			}
		}
		else if (selection_step == 2 && game_hosting == 1) // hosting multi player
		{
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int portbox_selected = CheckCollisionPointRec(GetMousePosition(), portBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->users[0], "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && portbox_selected)
				sprintf(portchar, "%s", clipboard);
			if (GuiTextBox(nickBox, data->users[0], 20, nickbox_selected) || GuiTextBox(portBox, portchar, 20, portbox_selected))
			{
				*PORT = atoi(portchar);
				sprintf(IP_ADDRESS, "127.0.0.1");
				game_running = 1;
				ret = 1;
			}
			DrawText("Nickname:", 10, 10, 20, DARKGRAY);
			DrawText("Port:", 10, 45, 20, DARKGRAY);
		}
		else if (selection_step == 3) // join multi player
		{
			int nickbox_selected = CheckCollisionPointRec(GetMousePosition(), nickBox);
			int ipbox_selected = CheckCollisionPointRec(GetMousePosition(), ipBox);
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && nickbox_selected)
				sprintf(data->users[0], "%s", clipboard);
			else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V) && ipbox_selected)
				sprintf(IP_ADDRESS, "%s", clipboard);
			if (GuiTextBox(nickBox, data->users[0], 20, nickbox_selected) || GuiTextBox(ipBox, IP_ADDRESS, 16, ipbox_selected))
			{
				if (strlen(IP_ADDRESS) <= 1)
					sprintf(IP_ADDRESS, "127.0.0.1");
				if (client_connect(IP_ADDRESS, 5555) != -1)
				{
					game_running = 1;
					ret = 0;
				}
				else
				{
					int tmp_time = time(NULL) + 3;
					ret = -1;
					while (time(NULL) != tmp_time)
					{
						BeginDrawing();
						DrawText(TextFormat("Error: connection to %s\non port %i failed!", IP_ADDRESS, 5555), 10, 10, 20, RED);
						ClearBackground(YELLOW);
						EndDrawing();
					}
					sprintf(IP_ADDRESS, " ");
				}
			}
			DrawText("Nickname:", 10, 10, 20, DARKGRAY);
			DrawText("IP:", 10, 45, 20, DARKGRAY);
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	CloseWindow();
	return ret;
}

void matchInfo(struct online_data *data)
{ // draw match info
	const char *info_text = TextFormat("It's %s %s turn!", data->turn ? data->users[1] : data->users[2], data->turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, 20)) / 2, block * 3 + 10, 20, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", data->users[1], data->winsP0, data->users[2], data->winsP1), 10, block * 3 + 40, 20, BLACK);
}