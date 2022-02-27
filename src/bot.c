#include "include/bot.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/main.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int game_running;

void *bot_main() {
	struct client_data data = (struct client_data){0};
	pthread_t tid[3];
	data.click_position[0] = -1;
	data.click_position[1] = -1;
	data.uid			   = -1;
	sprintf(data.username, "CPU");
	while (client_connect("127.0.0.1", 5555, &data.sockfd))
		;
	pthread_create(&tid[0], 0, client_comm, &data);
	pthread_create(&tid[1], 0, bot_ai, &data);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	return NULL;
}

void minimax(struct client_data *data) { // needs an actual implementation
	while (data->click_position[0] < 0 && data->click_position[0] < 0) {
		int rand_num[2] = {rand() % 3, rand() % 3};
		if (data->game_grid[rand_num[0]][rand_num[1]] == 0) {
			data->click_position[0] = rand_num[0];
			data->click_position[1] = rand_num[1];
		}
		data->ready = 1;
	}
	return;
}

void *bot_ai(void *arg) {
	struct client_data *data = (struct client_data *)arg;
	srand(time(NULL));
	while (game_running) {
		if (!data->is_game_over && !data->turn) {
			// usleep(rand() % 100000);
			minimax(data);
		}
		data->ready = 1;
	}
	return NULL;
}