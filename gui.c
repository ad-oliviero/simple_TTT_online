#include "lib/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_STATIC
#include "lib/raygui/src/raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/main.h"

extern int block;
extern int turn;
extern int winsP0;
extern int winsP1;
extern char IP_ADDRESS[15];
extern char user0[USERN_LENGTH];
extern char user1[USERN_LENGTH];
extern char user_name[USERN_LENGTH];
extern Rectangle game[9];

void initHitBox()
{ // creating boxes to detect touch or mouse clicks
	for (int i = 0; i < 9; i++)
	{
		game[i].height = block;
		game[i].width = block;
	}
	for (int i = 0; i < 3; i++)
	{
		game[i].x = 0 + i * block + 1;
		game[i].y = 0 + 1;
	}
	for (int i = 0; i < 3; i++)
	{
		game[i + 3].x = 0 + i * block + 1;
		game[i + 3].y = block + 1;
	}
	for (int i = 0; i < 3; i++)
	{
		game[i + 6].x = 0 + i * block + 1;
		game[i + 6].y = block * 2 + 1;
	}
}

void grid()
{
	// Borders
	Vector2 startp = {0, 1};
	Vector2 endp = {SCR_WIDTH, 1};
	DrawLineEx(startp, endp, THICKNESS, BLACK);
	startp.x = 1;
	endp.x = 1;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICKNESS, BLACK);
	startp.x = 0;
	startp.y = SCR_WIDTH;
	endp.x = SCR_WIDTH;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICKNESS, BLACK);
	startp.x = SCR_WIDTH;
	startp.y = 0;
	endp.x = SCR_WIDTH;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICKNESS, BLACK);

	// vertical grid
	startp.x = block;
	startp.y = 0;
	endp.x = block;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICKNESS, BLACK);
	startp.x = block * 2;
	startp.y = 0;
	endp.x = block * 2;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICKNESS, BLACK);

	// horizontal grid
	startp.x = 0;
	startp.y = block;
	endp.x = SCR_WIDTH;
	endp.y = block;
	DrawLineEx(startp, endp, THICKNESS, BLACK);
	startp.x = 0;
	startp.y = block * 2;
	endp.x = SCR_WIDTH;
	endp.y = block * 2;
	DrawLineEx(startp, endp, THICKNESS, BLACK);
}

void join_window()
{ // drawing the initial window
	SetTraceLogCallback(log_level);
	InitWindow(320, 75, "Set nickname and IP address");
	SetTargetFPS(GetMonitorRefreshRate(0));
	// i mostly took this part of the code from raylib.com/examples/web/text/loader.html?name=text_input_box, if you want to understand more just open the link
	Rectangle nickBox = {MeasureText("Nickname:", 20) + 15, 5, 200, 30};
	Rectangle ipBox = {MeasureText("IP:", 20) + 15, 40, 267, 30};
	while (!WindowShouldClose())
	{
		BeginDrawing();
		if (GuiTextBox(nickBox, user_name, 20, CheckCollisionPointRec(GetMousePosition(), nickBox)))
			break;
		if (GuiTextBox(ipBox, IP_ADDRESS, 16, CheckCollisionPointRec(GetMousePosition(), ipBox)))
			break;
		DrawText("Nickname:", 10, 10, 20, DARKGRAY);
		DrawText("IP:", 10, 45, 20, DARKGRAY);
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	printf("%lu\n", strlen(IP_ADDRESS));
	if (strlen(IP_ADDRESS) <= 1)
		sprintf(IP_ADDRESS, "127.0.0.1");
	CloseWindow();
	return;
}

void matchInfo()
{ // draw match info
	GuiTextAlignment align = 1;
	if (turn)
		DrawText(TextFormat("It's %s (X) turn!", user0), MeasureText("It's (X) turn!", 20), block * 3 + 10, 20, BLACK);
	else
		DrawText(TextFormat("It's %s (0) turn!", user1), MeasureText("It's (0) turn!", 20), block * 3 + 10, 20, BLACK);
	// DrawText("X:\nO:", 10, block * 3 + 10, 20, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", user0, winsP0, user1, winsP1), 10, block * 3 + 10, 20, BLACK);
}