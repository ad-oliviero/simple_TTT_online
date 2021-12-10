#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef __linux__
	#include <arpa/inet.h>
	#include <sys/socket.h>
#elif _WIN32
	#include <winsock2.h>
#endif
#include "include/main.h"

extern int game_running;

void print_data(struct client_data *data) {
	LOGI("Bot hardness: %i\nClick Position 0: %i\nClick Position 1: %i", data->bot_hardness, data->click_position[0], data->click_position[1]);
	LOGI("\nGame Grid:");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			printf("%i, ", data->game_grid[i][j]);
		printf("\n");
	}
	LOGI("Game Mode: %i\nButton Pressed: %i\nGame Over: %i\nReady: %i\nSockfd: %i\nTurn: %i\nUser ID: %i",
		 data->game_mode, data->is_btn_pressed, data->is_game_over, data->ready, data->sockfd, data->turn, data->user_id);
	printf("Users: ");
	for (int i = 0; i < 4; i++)
		printf("%s, ", data->users[i]);
	LOGI("\nWinner: %i\nWins Player 0: %i\nWins Player 1: %i", data->winner, data->winsP[0], data->winsP[1]);
	return;
}

int client_connect(char *IP_ADDRESS, int PORT, SOCK *sockfd) { // connect to the sock
	struct sockaddr_in servaddr;
	inet_pton(AF_INET, IP_ADDRESS, &servaddr.sin_addr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port	= htons(PORT);
	memset(servaddr.sin_zero, 0, sizeof(servaddr.sin_zero));
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOGE("Socket creation failed!");
		exit(-1);
	}

	if (connect(*sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
		LOGE("Connection to %s on port %i failed!", IP_ADDRESS, PORT);
		exit(-1);
	}
	return 0;
}

int client_recv_data(struct client_data *data) {
	// receiving normal variables
	/* if (recv(data->sockfd, &data->is_btn_pressed, sizeof(int), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	} */
	// if (recv(data->sockfd, &data->is_game_over, sizeof(int), 0) < 0) {
	// 	LOGE("Receive error, server could be offline.");
	// 	return -1;
	// }
	/* if (recv(data->sockfd, &data->ready, sizeof(int), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	} */
	// if (recv(data->sockfd, &data->turn, sizeof(int), 0) < 0) {
	// 	LOGE("Receive error, server could be offline.");
	// 	return -1;
	// }
	// if (recv(data->sockfd, &data->user_id, sizeof(int), 0) < 0) {
	// 	LOGE("Receive error, server could be offline.");
	// 	return -1;
	// }
	// if (recv(data->sockfd, &data->bot_hardness, sizeof(int), 0) < 0) {
	// 	LOGE("Receive error, server could be offline.");
	// 	return -1;
	// }
	/* if (recv(data->sockfd, &data->game_mode, sizeof(int), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	} */

	// receiving arrays
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (recv(data->sockfd, &data->game_grid[i][j], sizeof(int), 0) < 0) {
				LOGE("Receive error, server could be offline.");
				return -1;
			}
	// for (int i = 0; i < 2; i++) {
	// 	if (recv(data->sockfd, &data->click_position[i], sizeof(int), 0) < 0) {
	// 		LOGE("Receive error, server could be offline.");
	// 		return -1;
	// 	}
	// 	if (recv(data->sockfd, &data->winsP[i], sizeof(int), 0) < 0) {
	// 		LOGE("Receive error, server could be offline.");
	// 		return -1;
	// 	}
	// }
	// for (int i = 1; i < MAX_CLI; i++)
	// 	if (recv(data->sockfd, &data->users[i], sizeof(data->users[i]), 0) < 0) {
	// 		LOGE("Receive error, server could be offline.");
	// 		return -1;
	// 	}
	return 0;
}

int client_send_data(struct client_data *data) {
	// receiving normal variables
	if (send(data->sockfd, &data->ready, sizeof(int), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	}
	/* if (send(data->sockfd, &data->bot_hardness, sizeof(int), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	} */

	// sending arrays
	// for (int i = 0; i < 2; i++)
	if (send(data->sockfd, &data->click_position, sizeof(int) * 2, 0) < 0) {
		LOGE("Receive error, server could be offline.");
		return -1;
	}
	return 0;
}

void *client_comm(void *arg) { // communicating data with the server (mostly receiving)
	struct client_data *data = (struct client_data *)arg;

	// if (send(data->sockfd, data->users[0], strlen(data->users[0]), 0) < 0) {
	// 	LOGE("Failed to send username to server!");
	// 	return (void *)-1;
	// }

	while (game_running) {
		// LOGW("BEFORE");
		// print_data(data);
		if (client_recv_data(data) < 0) {
			LOGE("Receive error: server could be offline!");
			return (void *)-1;
		}
		if (data->turn == data->user_id) {
			data->click_position[0] = -1; // set click only if it's client's turn
			data->click_position[1] = -1;
		}
		if (client_send_data(data) < 0) {
			LOGE("Send error: server could be offline!");
			return (void *)-1;
		}

		// LOGW("AFTER");
		// print_data(data);
	}
	/* // initializing the game
	struct client_data *data = (struct client_data *)arg;
	// listen(data->sockfd, 10);
	write(data->sockfd, (char *)&data->users[0], sizeof(data->users[0]));
	read(data->sockfd, (char *)&data->users, sizeof(data->users));
	read(data->sockfd, (char *)&data->user_id, sizeof(data->user_id));
	if (data->user_id < 0) {
		LOGE("user_id %d\n", data->user_id);
		return -1;
	}
	LOGI("user_id: %d", data->user_id);
	// communication loop
	while (game_running) {
		// read game data
		read(data->sockfd, (char *)&data->is_game_over, sizeof(data->is_game_over));
		read(data->sockfd, (char *)&data->turn, sizeof(data->turn));
		read(data->sockfd, (char *)&data->winsP, sizeof(data->winsP));
		read(data->sockfd, (char *)&data->winner, sizeof(data->winner));
		read(data->sockfd, (char *)&data->game_grid, sizeof(data->game_grid));

		// checking if client has permission to play and sending data
		if (data->turn == data->user_id) {
			data->click_position[0] = -1; // set click only if it's client's turn
			data->click_position[1] = -1;
		}
		write(data->sockfd, (char *)&data->click_position, sizeof(data->click_position));
		if ((data->click_position[0] >= 0 && data->click_position[1] >= 0) || data->is_game_over) {
			data->click_position[0] = -1;
			data->click_position[1] = -1;
		}
		write(data->sockfd, (char *)&data->ready, sizeof(data->ready));
		if (data->ready == 1 && data->is_game_over == 0)
			data->ready = 0;
	}
	pthread_exit(NULL); */
}