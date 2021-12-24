#include "include/main.h"
#include "include/bot.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/server.h"
#include "include/shapes.h"
#include "lib/raylib/src/raylib.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int game_running = 0;
pthread_t tid[4];

int main() {
	struct client_data data	  = (struct client_data){0};
	struct server_data server = (struct server_data){0};
	data.click_position[0]	  = -1;
	data.click_position[1]	  = -1;
	data.user_id			  = -1;
	server.PORT				  = 5555;
	data.game_mode			  = join_window(server.IP_ADDRESS, &server.PORT, &data);
	if (data.game_mode < 0)
		return 0;
	else if (data.game_mode == 1 || data.game_mode == 2) {
		pthread_create(&tid[2], 0, server_main, &server);
		while (client_connect(server.IP_ADDRESS, server.PORT, &data.sock))
			;
	}
	if (data.game_mode == 2)
		sprintf(data.users[0], "Me");
	pthread_create(&tid[0], 0, client_comm, &data);
	// pthread_create(&tid[1], 0, window_main, &data);
	if (data.game_mode == 2)
		pthread_create(&tid[3], 0, bot_main, NULL);

	// main window
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, TextFormat("Simple TTT - %s", data.users[0]));
	SetTargetFPS(GetMonitorRefreshRate(0));
	initHitBox();
	while (!WindowShouldClose() && game_running) {
		place(&data);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				shape((int[2]){i, j}, &data.game_grid[i][j]);
		if (data.is_game_over == 1)
			end_client_game(&data);
		matchInfo(&data);
		EndDrawing();
	}
	// end of the program
	game_running = 0;
	CloseWindow();
	close(data.sock);
	return 0;
}
