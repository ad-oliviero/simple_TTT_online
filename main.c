#ifdef __linux__
#define SOCK int sock
#elif _WIN32
#define SOCK unsigned int sock
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/raylib/include/raylib.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/main.h"
#include "include/shapes.h"

extern SOCK;
int block = SCR_WIDTH / 3;
int game_running = 0;
struct online_data client_data = {0};
char user_name[32] = {0};
char user0[32];
char user1[32];
Rectangle game[9];
pthread_t tid[4];

int main()
{
	// struct online_data *data = (struct online_data *)malloc(sizeof(data));
	client_data.click_position = -1;
	if (join_window() != 0)
		return 0;
	// client_connect();
	pthread_create(&tid[0], 0, client_comm, NULL);
	pthread_create(&tid[1], 0, window_main, NULL);
	for (int i = 0; i <= 1; i++)
		pthread_join(tid[i], NULL);
	return 0;
}

void *window_main(/* void *arg_data */)
{
	// struct online_data *data = (struct online_data *)arg_data;
	// free(arg_data);
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, PROGRAM_NAME);
	SetTargetFPS(GetMonitorRefreshRate(0));
	initHitBox();
	// main game loop
	while (!WindowShouldClose() && game_running)
	{
		place();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 9; i++)
			shape(game, &i, &client_data.game_grid[i]);
		if (client_data.is_game_over == 1)
			end_client_game(client_data.winner);
		matchInfo();
		//DrawFPS(10, 10);
		EndDrawing();
	}
	pthread_cancel(tid[0]);
	game_running = 0;
	// end of the program
	CloseWindow();
	close(sock);
	pthread_exit(NULL);
	return NULL;
}
