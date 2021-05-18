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
		Vector2 startpa = {position[*position_id].x + 20, position[*position_id].y + 20};
		Vector2 endpa = {startpa.x + block - 40, startpa.y + block - 40};
		DrawLineEx(startpa, endpa, THICKNESS, BLACK);
		Vector2 startpb = {position[*position_id].x + block - 20, position[*position_id].y + 20};
		Vector2 endpb = {startpb.x - block + 40, startpb.y + block - 40};
		DrawLineEx(startpb, endpb, THICKNESS, BLACK);
	}
	else if (*type == 2)
	{
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / THICKNESS, BLACK);
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / (THICKNESS / 10 + 3), RAYWHITE);
	}
}