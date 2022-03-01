#include "../lib/raylib/src/raylib.h"
#include "include/client.h"
#include "include/gui.h"
#include "include/main.h"
#include <math.h>

Rectangle game[3][3];

// changes game_grid value where the click is detected (with the hitboxes in gui.c)
void place(struct client_data *data) {
	for (int i = 0; i < 9; i++)
		if (CheckCollisionPointRec(GetMousePosition(), game[i / 3][i % 3]) && IsMouseButtonDown(0) && !data->is_game_over) {
			data->click_position[0] = i / 3;
			data->click_position[1] = i % 3;
		}
}

// draw shape where game_grid value is != 0
void shape(int idx[2], int type) {
	if (type == 0)
		return;
	Vector2 pos = (Vector2){(BLOCK * idx[1]), (BLOCK * idx[0])};
	if (type == 1) {
		Vector2 dlen  = (Vector2){(BLOCK * (idx[0] + 1)) - pos.y, (BLOCK * (idx[1] + 1)) - pos.x};
		Vector2 dlend = (Vector2){dlen.x / 4, dlen.y / 4};
		DrawLineEx((Vector2){pos.x + dlend.x, pos.y + dlend.y}, (Vector2){pos.x + dlen.x - dlend.x, pos.y + dlen.y - dlend.y}, THICKNESS, BLACK);
		DrawLineEx((Vector2){pos.x + dlen.x - dlend.x, pos.y + dlend.y}, (Vector2){pos.x + dlend.x, pos.y + dlen.y - dlend.y}, THICKNESS, BLACK);
	} else if (type == 2) {
		Vector2 center = (Vector2){pos.x + (BLOCK / 2), pos.y + (BLOCK / 2)};
		float radius   = (BLOCK / 2) - (BLOCK / 6);
		DrawCircleV(center, radius, BLACK);
		DrawCircleV(center, radius - THICKNESS, RAYWHITE);
	}
}