#include <stdio.h>
#include <unistd.h>
#ifdef __linux__
	#include <netinet/in.h>
	#include <sys/socket.h>
#elif _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif
#include "include/gameplay.h"
#include "include/main.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>

int ready_check[MAX_CLI];
extern int game_running;

void *communication(void *);

void *server_main(void *arg) {
	struct server_data *servdata = (struct server_data *)arg;
	pthread_t t[MAX_CLI];
	SOCK listenfd;
	int opt = 1;
	struct sockaddr_in servaddr;
	servaddr.sin_family		 = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port		 = htons(servdata->PORT);
	memset(servaddr.sin_zero, 0, sizeof(servaddr.sin_zero));

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOGE("Socket creation failed!");
		return (void *)-1;
	}
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	if (bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0) {
		LOGE("Binding failed!");
		return (void *)-1;
	}
	if (listen(listenfd, 10) < 0) {
		LOGE("Error while calling listen()!");
		return (void *)-1;
	}
	for (int i = 0; i < 1 /* MAX_CLI */; i++) {
		servdata->clifd[i]	= accept(listenfd, NULL, NULL);
		servdata->thread_id = i;
		if (servdata->clifd[i] < 0) {
			LOGE("Failed to accept client %i!", i);
			return (void *)-1;
		}
		pthread_create(&t[i], 0, communication, servdata);
	}
	pthread_join(t[0], NULL);
	close(listenfd);
	return 0;
}

int server_send_data(struct client_data *data, SOCK *sockfd, int clid) {
	// sending normal variables
	/* if (send(*sockfd, &data->is_btn_pressed, sizeof(int), 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	} */
	// if (send(*sockfd, &data->is_game_over, sizeof(int), 0) < 0) {
	// 	LOGE("Error sending to client %i, disconnected!", clid);
	// 	return -1;
	// }
	/* if (send(*sockfd, &data->is_game_over, sizeof(int), 0) < 0) {
		 LOGE("Error sending to client %i, disconnected!", clid);
		 return (void *)-1;
	 } */
	/* if (send(*sockfd, &data->ready, sizeof(int), 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	} */
	// if (send(*sockfd, &data->turn, sizeof(int), 0) < 0) {
	// 	LOGE("Error sending to client %i, disconnected!", clid);
	// 	return -1;
	// }
	// if (send(*sockfd, &data->user_id, sizeof(int), 0) < 0) {
	// 	LOGE("Error sending to client %i, disconnected!", clid);
	// 	return -1;
	// }
	// if (send(*sockfd, &data->bot_hardness, sizeof(int), 0) < 0) {
	// 	LOGE("Error sending to client %i, disconnected!", clid);
	// 	return -1;
	// }
	/* if (send(*sockfd, &data->game_mode, sizeof(int), 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	} */

	// sending arrays
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (send(*sockfd, &data->game_grid[i][j], sizeof(int), 0) < 0) {
				LOGE("Error sending to client %i, disconnected!", clid);
				return -1;
			}
	// for (int i = 0; i < 2; i++) {
	// 	if (send(*sockfd, &data->click_position[i], sizeof(int), 0) < 0) {
	// 		LOGE("Error sending to client %i, disconnected!", clid);
	// 		return -1;
	// 	}
	// 	if (send(*sockfd, &data->winsP[i], sizeof(int), 0) < 0) {
	// 		LOGE("Error sending to client %i, disconnected!", clid);
	// 		return -1;
	// 	}
	// }
	// for (int i = 1; i < MAX_CLI; i++)
	// 	if (send(*sockfd, &data->users[i], strlen(data->users[i]), 0) < 0) {
	// 		LOGE("Error sending to client %i, disconnected!", clid);
	// 		return -1;
	// 	}
	return 0;
}

int server_recv_data(struct client_data *data, SOCK *sockfd, int clid) {
	// sending normal variables
	if (recv(*sockfd, &ready_check[clid], sizeof(int), 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	}
	/* if (recv(*sockfd, &data->bot_hardness, sizeof(int), 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	} */

	// receiving arrays
	// for (int i = 0; i < 2; i++)
	if (recv(*sockfd, &data->click_position, sizeof(int) * 2, 0) < 0) {
		LOGE("Error sending to client %i, disconnected!", clid);
		return -1;
	}
	return 0;
}

void *communication(void *arg) {
	struct server_data *servdata = (struct server_data *)arg;
	servdata->data.user_id		 = servdata->thread_id;
	servdata->client_running	 = 1;
	char buf[1000]				 = "";
	servdata->data.turn			 = 6;

	// if (recv(servdata->clifd[servdata->data.user_id], servdata->data.users[servdata->data.user_id+1], sizeof(servdata->data.users[servdata->data.user_id+1]), 0) < 0) {
	// 	LOGE("Receiving username from client %i failed!", servdata->data.user_id);
	// 	return (void *)-1;
	// }

	while (game_running) {
		servdata->data.winner = checkwinner(&servdata->data);
		if (server_send_data(&servdata->data, &servdata->clifd[servdata->data.user_id], servdata->data.user_id) < 0) {
			LOGE("Sending game data to client failed!");
			return (void *)-1;
		}

		if (server_recv_data(&servdata->data, &servdata->clifd[servdata->data.user_id], servdata->data.user_id) < 0) {
			LOGE("Receiving game data from client failed!");
			return (void *)-1;
		}

		LOGI("pos: %i %i\nturn: %i", servdata->data.click_position[0], servdata->data.click_position[1], servdata->data.turn);

		if (servdata->data.click_position[0] != -1 && servdata->data.click_position[0] != -1 &&
			servdata->data.game_grid[servdata->data.click_position[0]][servdata->data.click_position[1]] == 0 &&
			servdata->data.is_game_over == 0) { // handling click position
			if (servdata->data.turn)
				servdata->data.game_grid[servdata->data.click_position[0]][servdata->data.click_position[1]] = 1;
			else
				servdata->data.game_grid[servdata->data.click_position[0]][servdata->data.click_position[1]] = 2;
			servdata->data.turn = !servdata->data.turn;
		}
		if (servdata->data.turn != servdata->data.user_id) {
			servdata->data.click_position[0] = -1;
			servdata->data.click_position[1] = -1;
		}
		if (ready_check[0] && ready_check[1])
			end_server_game(servdata->data.winner, &servdata->data); // checks if all clients are ready to continue
	}
	close(servdata->clifd[servdata->data.user_id]);
	return 0;
	// communicating server_data->data with the client (mostly sending)
	/* struct server_data *server_data = (struct server_data *)arg;
	int client_id					= server_data->thread_id;
	server_data->client_running		= 1;
	while (game_running) {
		// server_data->data.turn = !server_data->data.turn;
		server_data->data.winner = checkwinner(&server_data->data);

		// write server_data->data to client
		write(clientfd[client_id], (const char *)&server_data->data.is_game_over, sizeof(server_data->data.is_game_over));
		write(clientfd[client_id], (const char *)&server_data->data.turn, sizeof(server_data->data.turn));
		write(clientfd[client_id], (const char *)&server_data->data.winsP, sizeof(server_data->data.winsP));
		write(clientfd[client_id], (const char *)&server_data->data.winner, sizeof(server_data->data.winner));
		write(clientfd[client_id], (const char *)&server_data->data.game_grid, sizeof(server_data->data.game_grid));

		// read client events
		read(clientfd[client_id], (char *)&server_data->data.click_position, sizeof(server_data->data.click_position));
		read(clientfd[client_id], (char *)&ready_check[client_id], sizeof(ready_check));

		// check if client has permission to play
		if (server_data->data.turn == client_id) {
			server_data->data.click_position[0] = (void*)-1;
			server_data->data.click_position[1] = (void*)-1;
		}																																																												   // accepting click_position only from player's turn client
		if (server_data->data.click_position[0] != (void*)-1 && server_data->data.click_position[1] != (void*)-1 && server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] == 0 && server_data->data.is_game_over == 0) { // handling click_positions
			if (server_data->data.turn)
				server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] = 1;
			else
				server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] = 2;
			server_data->data.turn = !server_data->data.turn;
		}
		if (ready_check[0] && ready_check[1])
			end_server_game(server_data->data.winner, &server_data->data); // checks if all clients are ready to continue
	}
	close(clientfd[client_id]);
	pthread_exit(NULL); */
}