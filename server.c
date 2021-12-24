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

int clientfd[4], server_fd;
int ready_check[4];
extern int game_running;

void *communication(void *);

void *server_main(void *arg) {
	struct server_data *server_data = (struct server_data *)arg;
	server_data->server_tid			= pthread_self();
	pthread_t server_tid[128];

	// creating socket and connecting to it
#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#endif
	struct sockaddr_in address;
	int opt = 1, addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port		= htons(server_data->PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return NULL;

	// accepting clients
	listen(server_fd, 3);
	clientfd[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	recv(clientfd[0], &server_data->data.users[1], sizeof(server_data->data.users[1]), 0);

	listen(server_fd, 3);
	clientfd[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	recv(clientfd[1], &server_data->data.users[2], sizeof(server_data->data.users[2]), 0);

	// initializing connection
	listen(server_fd, 3);
	send(clientfd[0], &server_data->data.users, sizeof(server_data->data.users), 0);
	send(clientfd[0], &server_data->data.turn, sizeof(server_data->data.turn), 0);
	server_data->data.turn++;
	send(clientfd[1], &server_data->data.users, sizeof(server_data->data.users), 0);
	send(clientfd[1], &server_data->data.turn, sizeof(server_data->data.turn), 0);

	// creating and joining threads
	while (server_data->thread_id <= 1) {
		server_data->client_running = 0;
		pthread_create(&server_tid[server_data->thread_id], 0, communication, server_data);
		while (server_data->client_running == 0)
			;
		server_data->thread_id++;
	}
	for (int i = 0; i <= 1; i++)
		pthread_join(server_tid[i], NULL);
	return 0;
}

void *communication(void *arg) { // communicating server_data->data with the client (mostly sending)
	struct server_data *server_data = (struct server_data *)arg;
	int client_id					= server_data->thread_id;
	server_data->client_running		= 1;
	while (game_running) {
		// server_data->data.turn = !server_data->data.turn;
		server_data->data.winner = checkwinner(&server_data->data);

		// send server_data->data to client
		send(clientfd[client_id], &server_data->data.is_game_over, sizeof(server_data->data.is_game_over), 0);
		send(clientfd[client_id], &server_data->data.turn, sizeof(server_data->data.turn), 0);
		send(clientfd[client_id], &server_data->data.winsP, sizeof(server_data->data.winsP), 0);
		send(clientfd[client_id], &server_data->data.winner, sizeof(server_data->data.winner), 0);
		send(clientfd[client_id], &server_data->data.game_grid, sizeof(server_data->data.game_grid), 0);

		// recv client events
		recv(clientfd[client_id], &server_data->data.click_position, sizeof(server_data->data.click_position), 0);
		recv(clientfd[client_id], &ready_check[client_id], sizeof(ready_check), 0);

		// check if client has permission to play
		if (server_data->data.turn == client_id) {
			server_data->data.click_position[0] = -1;
			server_data->data.click_position[1] = -1;
		}																																																												   // accepting click_position only from player's turn client
		if (server_data->data.click_position[0] != -1 && server_data->data.click_position[1] != -1 && server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] == 0 && server_data->data.is_game_over == 0) { // handling click_positions
			if (server_data->data.turn)
				server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] = 1;
			else
				server_data->data.game_grid[server_data->data.click_position[0]][server_data->data.click_position[1]] = 2;
			server_data->data.turn = !server_data->data.turn;
		}
		if (ready_check[0] && ready_check[1])
			end_server_game(server_data->data.winner, &server_data->data); // checks if all clients are recvy to continue
	}
	close(clientfd[client_id]);
	pthread_exit(NULL);
}