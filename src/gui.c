#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/main.h"

extern int block;
extern Rectangle game[9];
extern int turn;
extern int winsP1;
extern int winsP2;
extern char user1[10];
extern char user2[10];
extern char user_name[10];
extern char IP_ADDRESS[60];

void join_window() {
	SetTraceLogCallback(log_level);
	InitWindow(320, 75, "Nickname and IP");
	SetTargetFPS(GetMonitorRefreshRate(0));
	Rectangle nickBox = {MeasureText("Nickname:", 20) + 15, 5, 200, 30};
	Rectangle ipBox = {MeasureText("IP:", 20) + 15, 40, 267, 30};
	int letterCountNick = 0, letterCountIP = 0;
	while (!IsKeyDown(KEY_ENTER)) {
		if (CheckCollisionPointRec(GetMousePosition(), nickBox)) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			int key = GetCharPressed();
			while (key > 0) {
				if ((key >= 32) && (key <= 125) && (letterCountNick < 10)) {
					user_name[letterCountNick] = (char)key;
					letterCountNick++;
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE)) {
				letterCountNick--;
				if (letterCountNick < 0) letterCountNick = 0;
				user_name[letterCountNick] = '\0';
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), ipBox)) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			int key = GetCharPressed();
			while (key > 0) {
				if ((key >= 32) && (key <= 125) && (letterCountIP < 15)) {
					IP_ADDRESS[letterCountIP] = (char)key;
					letterCountIP++;
				}
				key = GetCharPressed();
			}
			if (IsKeyPressed(KEY_BACKSPACE)) {
				letterCountIP--;
				if (letterCountIP < 0) letterCountIP = 0;
				IP_ADDRESS[letterCountIP] = '\0';
			}
		}
		BeginDrawing();
		DrawText("Nickname:", 10, 10, 20, DARKGRAY);
		DrawText("IP:", 10, 45, 20, DARKGRAY);
		DrawRectangleRec(nickBox, LIGHTGRAY);
		DrawRectangleRec(ipBox, LIGHTGRAY);
		DrawText(user_name, nickBox.x + 5, nickBox.y + 4, 20, MAROON);
		DrawText(IP_ADDRESS, ipBox.x + 5, ipBox.y + 4, 20, MAROON);
		if (CheckCollisionPointRec(GetMousePosition(), nickBox)) {
			if (letterCountNick < 10) {
				DrawText("|", nickBox.x + 8 + MeasureText(user_name, 20), nickBox.y + 6, 20, MAROON);
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), ipBox)) {
			if (letterCountIP < 15) {
				DrawText("|", ipBox.x + 8 + MeasureText(IP_ADDRESS, 20), ipBox.y + 6, 20, MAROON);
			}
		}
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	CloseWindow();
	if (letterCountIP < 5) {
		strcpy(IP_ADDRESS, "127.0.0.1");
	}
	return;
}

void grid() {
	// Borders
	Vector2 startp = {0, 1};
	Vector2 endp = {SCR_WIDTH, 1};
	DrawLineEx(startp, endp, THICCNESS, BLACK);
	startp.x = 1;
	endp.x = 1;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICCNESS, BLACK);
	startp.x = 0;
	startp.y = SCR_WIDTH;
	endp.x = SCR_WIDTH;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICCNESS, BLACK);
	startp.x = SCR_WIDTH;
	startp.y = 0;
	endp.x = SCR_WIDTH;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICCNESS, BLACK);

	// vertical grid
	startp.x = block;
	startp.y = 0;
	endp.x = block;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICCNESS, BLACK);
	startp.x = block * 2;
	startp.y = 0;
	endp.x = block * 2;
	endp.y = SCR_WIDTH;
	DrawLineEx(startp, endp, THICCNESS, BLACK);

	// horizontal grid
	startp.x = 0;
	startp.y = block;
	endp.x = SCR_WIDTH;
	endp.y = block;
	DrawLineEx(startp, endp, THICCNESS, BLACK);
	startp.x = 0;
	startp.y = block * 2;
	endp.x = SCR_WIDTH;
	endp.y = block * 2;
	DrawLineEx(startp, endp, THICCNESS, BLACK);
}

void initHitBox() {
	// creating boxes to detect touch or mouse clicks
	for (int i = 0; i < 9; i++) {
		game[i].height = block;
		game[i].width = block;
	}
	for (int i = 0; i < 3; i++) {
		game[i].x = 0 + i * block + 1;
		game[i].y = 0 + 1;
	}
	for (int i = 0; i < 3; i++) {
		game[i + 3].x = 0 + i * block + 1;
		game[i + 3].y = block + 1;
	}
	for (int i = 0; i < 3; i++) {
		game[i + 6].x = 0 + i * block + 1;
		game[i + 6].y = block * 2 + 1;
	}
}

void matchInfo() {
	if (turn) {
		DrawText(TextFormat("It's %s (X) turn!", user1), 150/*MeasureText(TextFormat("It's %s (X) turn!", user1), 20) / 5*/, block * 3 + 10, 20, BLACK);
	} else if (turn == 0) {
		DrawText(TextFormat("It's %s (0) turn!", user2), 150/*MeasureText(TextFormat("It's %s (0) turn!", user2), 20) / 5*/, block * 3 + 10, 20, BLACK);
	}
	DrawText("X:\nO:", 10, block * 3 + 10, 20, BLACK);
	DrawText(TextFormat("   %i\n   %i\n", winsP1, winsP2), 10, block * 3 + 10, 20, BLACK);
}