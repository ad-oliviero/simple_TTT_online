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
#include "include/main.h"
#include "include/client.h"

extern int block;
extern struct online_data client_data;
extern int game_running;
extern char user0[32];
extern char user1[32];
extern char user_name[32];
extern Rectangle game[9];

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

int join_window()
{
	int ret = 1;
	SetTraceLogLevel(LOG_NONE);
	InitWindow(320, 75, "Set nickname and IP address");
	SetTargetFPS(GetMonitorRefreshRate(0));
	Rectangle nickBox = {MeasureText("Nickname:", 20) + 15, 5, 200, 30};
	Rectangle ipBox = {MeasureText("IP:", 20) + 15, 40, 267, 30};
	char IP_ADDRESS[16] = {0};
	while (!game_running && !WindowShouldClose())
	{
		BeginDrawing();
		if ((GuiTextBox(nickBox, user_name, 20, CheckCollisionPointRec(GetMousePosition(), nickBox)) || GuiTextBox(ipBox, IP_ADDRESS, 16, CheckCollisionPointRec(GetMousePosition(), ipBox))))
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
				ret = 1;
				while (time(NULL) != tmp_time)
				{
					BeginDrawing();
					DrawText(TextFormat("Error: connection to %s\non port %i failed!", IP_ADDRESS, 5555), 10, 10, 20, RED);
					ClearBackground(YELLOW);
					EndDrawing();
				}
				sprintf(IP_ADDRESS, "");
			}
		}
		DrawText("Nickname:", 10, 10, 20, DARKGRAY);
		DrawText("IP:", 10, 45, 20, DARKGRAY);
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	CloseWindow();
	return ret;
}

void matchInfo(/* struct online_data *data */)
{ // draw match info
	GuiTextAlignment align = 1;
	const char *info_text = TextFormat("It's %s %s turn!", client_data.turn ? user0 : user1, client_data.turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, 20)) / 2, block * 3 + 10, 20, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", user0, client_data.winsP0, user1, client_data.winsP1), 10, block * 3 + 40, 20, BLACK);
}