#include "../lib/raylib/src/raylib.h"
#include "include/client.h"
#include "include/gui.h"
#include "include/main.h"

Rectangle game[3][3];

// changes game_grid value where the click is detected (with the hitboxes in gui.c)
void place(struct client_data *data) {
	for (int i = 0; i < 9; i++)
		if (CheckCollisionPointRec(
#ifdef __ANDROID_API__
				get_touch_pos(),
#else
				GetMousePosition(),
#endif
				game[i / 3][i % 3]) &&
			IsMouseButtonDown(0) && !data->is_game_over) {
			data->click_position[0] = i / 3;
			data->click_position[1] = i % 3;
		}
}

// draw shape where game_grid value is != 0
void shape(int idx[2], int *type) {
	if (*type == 1) {
		DrawLineEx((Vector2){game[idx[0]][idx[1]].x + 20, game[idx[0]][idx[1]].y + 20}, (Vector2){game[idx[0]][idx[1]].x + BLOCK - 20, game[idx[0]][idx[1]].y + BLOCK - 20}, THICKNESS, BLACK);
		DrawLineEx((Vector2){game[idx[0]][idx[1]].x + BLOCK - 20, game[idx[0]][idx[1]].y + 20}, (Vector2){game[idx[0]][idx[1]].x + BLOCK - BLOCK + 20, game[idx[0]][idx[1]].y + BLOCK - 20}, THICKNESS, BLACK);
	} else if (*type == 2) {
		DrawCircle(game[idx[0]][idx[1]].x + BLOCK / 2, game[idx[0]][idx[1]].y + BLOCK / 2, BLOCK / THICKNESS + 10, BLACK);
		DrawCircle(game[idx[0]][idx[1]].x + BLOCK / 2, game[idx[0]][idx[1]].y + BLOCK / 2, BLOCK / THICKNESS + 7, RAYWHITE);
	}
}