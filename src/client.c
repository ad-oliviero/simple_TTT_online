#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(__linux__) || defined(__EMSCRIPTEN__)
	#include <arpa/inet.h>
	#include <sys/socket.h>
#elif _WIN32
	#include <winsock2.h>
#endif
#include "include/main.h"

extern int game_running;

int client_connect(char *IP_ADDRESS, int PORT, SOCK *sock) { // connect to the sock
#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#endif
	struct sockaddr_in server_id;
	server_id.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_id.sin_family	  = AF_INET;
	server_id.sin_port		  = htons(PORT);
	*sock					  = socket(AF_INET, SOCK_STREAM, 0);
	return connect(*sock, (struct sockaddr *)&server_id, sizeof(server_id));
}

void *client_comm(void *arg) { // communicating data with the server (mostly receiving)
	// initializing the game
	struct client_data *data = (struct client_data *)arg;
	listen(data->sockfd, 1);
	send(data->sockfd, &data->username, sizeof(data->username), 0);
	recv(data->sockfd, &data->uid, sizeof(data->uid), 0);

	// communication loop
	while (game_running) {
		// recv game data
		recv(data->sockfd, &data->users, sizeof(data->users), 0);
		recv(data->sockfd, &data->is_game_over, sizeof(data->is_game_over), 0);
		recv(data->sockfd, &data->turn, sizeof(data->turn), 0);
		recv(data->sockfd, &data->winsP, sizeof(data->winsP), 0);
		recv(data->sockfd, &data->winner, sizeof(data->winner), 0);
		recv(data->sockfd, &data->game_grid, sizeof(data->game_grid), 0);

		// checking if client has permission to play and sending data
		if (data->turn == data->uid) {
			data->click_position[0] = -1; // set click only if it's client's turn
			data->click_position[1] = -1;
		}
		send(data->sockfd, &data->click_position, sizeof(data->click_position), 0);
		if ((data->click_position[0] >= 0 && data->click_position[1] >= 0) || data->is_game_over) {
			data->click_position[0] = -1;
			data->click_position[1] = -1;
		}
		send(data->sockfd, &data->ready, sizeof(data->ready), 0);
		if (data->ready == 1 && data->is_game_over == 0)
			data->ready = 0;
	}
	return NULL;
}