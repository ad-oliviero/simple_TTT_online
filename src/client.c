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
	*sock					  = socket(AF_INET, SOCK_STREAM, 6); // 6 = tcp protocol
	return connect(*sock, (struct sockaddr *)&server_id, sizeof(server_id));
}

void *client_comm(void *arg) { // communicating data with the server (mostly receiving)
	// initializing the game
	struct client_data *data = (struct client_data *)arg;
	listen(data->sockfd, 1);
	for (int i = 0; i < 32; i++)
		send(data->sockfd, &(data->username[i]), sizeof(data->username[i]), 0);
	int ibuf = 0;
	recv(data->sockfd, &ibuf, sizeof(ibuf), 0);
	data->uid = ntohl(ibuf);

	int recv_flags = 0;
	int send_flags = 0;

	// communication loop
	while (game_running) {
		// recv game data
		for (int i = 0; i < 4; i++)
			recv(data->sockfd, &(data->users[i]), 32, recv_flags);
		// receive data in network byte order and convert it to host byte order
		recv(data->sockfd, &ibuf, sizeof(ibuf), recv_flags);
		data->is_game_over = ntohl(ibuf);
		recv(data->sockfd, &ibuf, sizeof(ibuf), recv_flags);
		data->turn = ntohl(ibuf);
		recv(data->sockfd, &ibuf, sizeof(ibuf), recv_flags);
		data->winsP[0] = ntohl(ibuf);
		recv(data->sockfd, &ibuf, sizeof(ibuf), recv_flags);
		data->winsP[1] = ntohl(ibuf);
		recv(data->sockfd, &ibuf, sizeof(ibuf), recv_flags);
		data->winner = ntohl(ibuf);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				recv(data->sockfd, &(ibuf), sizeof(ibuf), recv_flags);
				data->game_grid[i][j] = ntohl(ibuf);
			}

		// checking if client has permission to play and sending data
		if (data->turn == data->uid) {
			data->click_position[0] = -1; // set click only if it's client's turn
			data->click_position[1] = -1;
		}
		ibuf = htonl(data->click_position[0]);
		send(data->sockfd, &ibuf, sizeof(ibuf), send_flags);
		ibuf = htonl(data->click_position[1]);
		send(data->sockfd, &ibuf, sizeof(ibuf), send_flags);
		if ((data->click_position[0] != -1 && data->click_position[1] != -1) || data->is_game_over == 1) {
			data->click_position[0] = -1;
			data->click_position[1] = -1;
		}
		ibuf = htonl(data->ready);
		send(data->sockfd, &ibuf, sizeof(ibuf), send_flags);
		if (data->ready == 1 && data->is_game_over == 0)
			data->ready = 0;
	}
	return NULL;
}