#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
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
int turn = 0;
int winsP0 = 0;
int winsP1 = 0;
int is_btn_pressed = 0;
int click_position = -1;
int winner = 0;
int ready = 0;
int permisson;
char user_name[USERN_LENGTH] = {0};
void log_level(){}
char user0[USERN_LENGTH];
char user1[USERN_LENGTH];

void* window_main();

int main() {
	join_window();
	if (client_connect() > 0) return 1;
	pthread_t tid[2];
	pthread_create(&tid[0], 0, client_comm, NULL);
	pthread_create(&tid[1], 0, window_main, NULL);
	for (int i = 0; i <= 1; i++) pthread_join(tid[i], NULL);
	return 0;
}

void* window_main() {
	SetTraceLogCallback(log_level);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, PROGRAM_NAME);
	SetTargetFPS(GetMonitorRefreshRate(0));
	initHitBox();
	while (!WindowShouldClose()) {
		place();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 9; i++) {
			shape(game, &i, &game_grid[i]);
		}
		if (is_game_over == 1) {
			endGame(winner);
		}
		matchInfo();
		DrawFPS(10, 10);
		EndDrawing();
	}
	CloseWindow();
	close(sock);
	pthread_exit(NULL);
}
