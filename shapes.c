#include "include/client.h"
#include "include/main.h"
#include "lib/raylib/include/raylib.h"

extern Rectangle game[9];

void place(struct client_data *data) { // changes game_grid value where the click is detected (with the hitboxes in gui.c)
	for (int i = 0; i < 9; i++)
		if (CheckCollisionPointRec(GetMousePosition(), game[i]) && IsMouseButtonDown(0) && !data->is_game_over) {
			data->click_position[0] = i / 3;
			data->click_position[1] = i % 3;
		}
}

void shape(Rectangle *position, int *position_id, int *type) { // draw shape where game_grid value is != 0
	if (*type == 1) {
		DrawLineEx((Vector2){position[*position_id].x + 20, position[*position_id].y + 20}, (Vector2){position[*position_id].x + BLOCK - 20, position[*position_id].y + BLOCK - 20}, THICKNESS, BLACK);
		DrawLineEx((Vector2){position[*position_id].x + BLOCK - 20, position[*position_id].y + 20}, (Vector2){position[*position_id].x + BLOCK - BLOCK + 20, position[*position_id].y + BLOCK - 20}, THICKNESS, BLACK);
	} else if (*type == 2) {
		DrawCircle(position[*position_id].x + BLOCK / 2, position[*position_id].y + BLOCK / 2, BLOCK / THICKNESS + 10, BLACK);
		DrawCircle(position[*position_id].x + BLOCK / 2, position[*position_id].y + BLOCK / 2, BLOCK / THICKNESS + 7, RAYWHITE);
	}
}