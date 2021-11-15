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
#include "include/server.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/main.h"
#include "include/shapes.h"

extern SOCK;
int block = SCR_WIDTH / 3;
int game_running = 0;
char user_name[32] = {0};
char user0[32];
char user1[32];
Rectangle game[9];
pthread_t tid[3];

int main()
{
	struct online_data data = (struct online_data){0};
	data.click_position = -1;
	int PORT = 5555;
	char IP_ADDR[16] = {0};
	int join_game = join_window(IP_ADDR, &PORT);
	if (join_game < 0)
		return 0;
	else if (join_game == 1)
	{
		printf("%d\n", PORT);
		pthread_create(&tid[2], 0, server_main, &PORT);
		while (client_connect(IP_ADDR, PORT))
			;
	}
	pthread_create(&tid[0], 0, client_comm, &data);
	pthread_create(&tid[1], 0, window_main, &data);
	for (int i = 0; i <= 1; i++)
		pthread_join(tid[i], NULL);
	if (join_game == 1)
		pthread_join(tid[2], NULL);
	return 0;
}

void *window_main(void *arg_data)
{
	struct online_data *data = (struct online_data *)arg_data;
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, TextFormat("Simple TTT - %s", user_name));
	SetTargetFPS(GetMonitorRefreshRate(0));
	initHitBox();
	// main game loop
	while (!WindowShouldClose() && game_running)
	{
		place(data);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 9; i++)
			shape(game, &i, &data->game_grid[i]);
		if (data->is_game_over == 1)
			end_client_game(data);
		matchInfo(data);
		//DrawFPS(10, 10);
		EndDrawing();
	}
	game_running = 0;
	// end of the program
	CloseWindow();
	close(sock);
	pthread_exit(NULL);
	return NULL;
}
