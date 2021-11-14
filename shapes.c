#include "lib/raylib/include/raylib.h"
#include "include/main.h"
#include "include/client.h"

extern int block;
extern int click_position;
extern int game_grid[9];
extern int is_game_over;
extern int turn;
extern Rectangle game[9];

void place()
{ // changes game_grid value where the click is detected (with the hitboxes in gui.c)
	for (int i = 0; i < 9; i++)
		if (CheckCollisionPointRec(GetMousePosition(), game[i]) && IsMouseButtonDown(0) && !is_game_over)
			click_position = i;
}

void shape(Rectangle *position, int *position_id, int *type)
{ // draw shape where game_grid value is != 0
	if (*type == 1)
	{
		DrawLineEx((Vector2){position[*position_id].x + 20, position[*position_id].y + 20}, (Vector2){position[*position_id].x + block - 20, position[*position_id].y + block - 20}, THICKNESS, BLACK);
		DrawLineEx((Vector2){position[*position_id].x + block - 20, position[*position_id].y + 20}, (Vector2){position[*position_id].x + block - block + 20, position[*position_id].y + block - 20}, THICKNESS, BLACK);
	}
	else if (*type == 2)
	{
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / THICKNESS + 10, BLACK);
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / THICKNESS + 7, RAYWHITE);
	}
}