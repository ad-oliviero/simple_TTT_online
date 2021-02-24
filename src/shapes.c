#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include "include/main.h"
#include "include/client.h"

extern int block;
extern int click_position;
extern int game_grid[9];
extern int is_game_over;
extern bool turn;

void shape(Rectangle *position, int *position_id, int *type) {
	if (*type == 1) {
		Vector2 startpa = {position[*position_id].x + 20, position[*position_id].y + 20};
		Vector2 endpa = {startpa.x + block - 40, startpa.y + block - 40};
		DrawLineEx(startpa, endpa, THICCNESS, BLACK);
		Vector2 startpb = {position[*position_id].x + block - 20, position[*position_id].y + 20};
		Vector2 endpb = {startpb.x - block + 40, startpb.y + block - 40};
		DrawLineEx(startpb, endpb, THICCNESS, BLACK);
	} else if (*type == 2) {
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / THICCNESS, BLACK);
		DrawCircle(position[*position_id].x + block / 2, position[*position_id].y + block / 2, block / (THICCNESS / 10 + 3), RAYWHITE);
	}
}

void cross(Rectangle *game) {
	for (int i = 0; i < 9; i++) {
		if (CheckCollisionPointRec(GetMousePosition(), game[i]) && IsMouseButtonDown(0) && is_game_over == 0) {
			click_position = i;
		}
	}
}

void circle(Rectangle *game) {
	for (int i = 0; i < 9; i++) {
		if (CheckCollisionPointRec(GetMousePosition(), game[i]) && game_grid[i] == 0 && IsMouseButtonDown(0) && is_game_over == 0) {
			click_position = i;
		}
	}
}