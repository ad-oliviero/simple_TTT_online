#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/main.h"
#include "include/shapes.h"

extern int sock;
int block = SCR_WIDTH / 3;
Rectangle game[9];
int game_grid[9] = {
	0, 0, 0,
	0, 0, 0,
	0, 0, 0
};
int is_game_over = 0;
int turn = 1;
int winsP1 = 0;
int winsP2 = 0;
int is_btn_pressed = 0;
int click_position = -1;
int winner = 0;
int ready = 0;
int permisson;
char user_name[10] = {0};
void log_level(){}
char user1[10];
char user2[10];

int main() {
	join_window();
	if (client_connect() > 0) return 1;
	SetTraceLogCallback(log_level);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, PROGRAM_NAME);
	SetTargetFPS(GetMonitorRefreshRate(0));
	
	initHitBox();
	// ---MAIN GAME LOOP---
	while (!WindowShouldClose()) {
		client_comm();
		if (turn) {
			cross(game);
		} else {
			circle(game);
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 9; i++) {
			shape(game, &i, &game_grid[i]);
		}
		if (is_game_over == 1) {
			endGame(winner);
		}
		//DrawFPS(10, 10);
		matchInfo();
		EndDrawing();
	}
	CloseWindow();
	close(sock);
	return 0;
}
