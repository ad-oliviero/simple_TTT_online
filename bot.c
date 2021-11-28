#include "include/bot.h"
#include "include/client.h"
#include "include/main.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int game_running;

void *bot_main() {
	struct client_data data = (struct client_data){0};
	pthread_t tid[3];
	data.click_position = -1;
	data.user_id		= -1;
	sprintf(data.users[0], "CPU");
	while (client_connect("127.0.0.1", 5555, &data.sock))
		;
	pthread_create(&tid[0], 0, client_comm, &data);
	pthread_create(&tid[1], 0, bot_ai, &data);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	return NULL;
}

void *bot_ai(void *arg) {
	struct client_data *data = (struct client_data *)arg;
	// main game loop
	srand(time(NULL));
	while (game_running) {
		if (!data->is_game_over && !data->turn) {
			usleep(rand() % 100000);
			if (data->bot_hardness == 0)
				data->click_position = rand() % 9;
			else if (data->bot_hardness == 1) {
			}
		}
		data->ready = (data->is_game_over == 1);
	}
	game_running = 0;
	return NULL;
}