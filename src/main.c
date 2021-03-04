#ifdef __linux__
	#define SOCK	int sock
#elif _WIN32
	#define SOCK	unsigned int sock
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/raylib.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/main.h"
#include "include/shapes.h"

extern SOCK;
int block = SCR_WIDTH / 3;
int game_grid[9] = {0};
int is_btn_pressed = 0;
int is_game_over = 0;
int click_position = -1;
int ready = 0;
int turn = 0;
int winsP0 = 0;
int winsP1 = 0;
int winner = 0;
char user_name[USERN_LENGTH] = {0};
char user0[USERN_LENGTH];
char user1[USERN_LENGTH];
Rectangle game[9];
void log_level(){}

int main() {
	join_window();
	client_connect();
	pthread_t tid[4];
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
	// main game loop
	while (!WindowShouldClose()) {
		place();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 9; i++) shape(game, &i, &game_grid[i]);
		if (is_game_over == 1) endGame(winner);
		matchInfo();
		//DrawFPS(10, 10);
		EndDrawing();
	}
	// end of the program
	CloseWindow();
	close(sock);
	pthread_exit(NULL);
	return NULL;
}
