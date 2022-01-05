#include "include/main.h"
#include "../lib/raylib/src/raylib.h"
#include "include/bot.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/server.h"
#include "include/shapes.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int game_running = 0;

int main() {
	struct client_data *data   = calloc(1, sizeof(struct client_data));
	struct server_data *server = calloc(1, sizeof(struct server_data));
	data->click_position[0]	   = -1;
	data->click_position[1]	   = -1;
	data->uid				   = -1;
	server->PORT			   = 5555;
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, "Game Mode Selection");
	SetTargetFPS(GetMonitorRefreshRate(0));
#ifdef __ANDROID_API__
	SCR_WIDTH  = GetScreenWidth();
	SCR_HEIGHT = GetScreenHeight();
#endif
	data->game_mode = join_window(server->IP_ADDRESS, &server->PORT, data);
	pthread_t tid[4];
	if (data->game_mode < 0)
		return 0;
	else if (data->game_mode == 1 || data->game_mode == 2) {
		pthread_create(&tid[2], 0, server_main, server);
		while (client_connect(server->IP_ADDRESS, server->PORT, &data->sockfd))
			;
	}
	if (data->game_mode == 2)
		sprintf(data->username, "Me");
	pthread_create(&tid[0], 0, client_comm, data);
	if (data->game_mode == 2)
		pthread_create(&tid[3], 0, bot_main, NULL);

	// main window
	initHitBox();
	SetWindowTitle(TextFormat("Simple TTT - %s", data->username));
	while (!WindowShouldClose() && game_running) {
		place(data);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				shape((int[2]){i, j}, &data->game_grid[i][j]);
		if (data->is_game_over == 1)
			end_client_game(data);
		matchInfo(data);
		EndDrawing();
	}
	// end of the program
	game_running = 0;
	CloseWindow();
	close(data->sockfd);
	return 0;
}
