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

void bot_easy(struct client_data *data) {
	for (int i = 0; i < 9 && data->click_position < 0; i++) {
		int rand_num = rand() % 9;
		if (data->game_grid[rand_num] == 0) {
			data->click_position = rand_num;
		}
	}
}
void bot_medium(struct client_data *data) { bot_easy(data); }
void bot_hard(struct client_data *data) { bot_medium(data); }
void bot_impossible(struct client_data *data) { bot_hard(data); }

void *bot_ai(void *arg) {
	struct client_data *data = (struct client_data *)arg;
	srand(time(NULL));
	while (game_running) {
		if (!data->is_game_over && !data->turn) {
			usleep(rand() % 100000);
			switch (data->bot_hardness) {
			case 1:
				bot_medium(data);
				break;

			case 2:
				bot_hard(data);
				break;

			case 3:
				bot_impossible(data);
				break;

			default:
				bot_easy(data);
				break;
			}
		}
		data->ready = (data->is_game_over == 1);
	}
	game_running = 0;
	return NULL;
}