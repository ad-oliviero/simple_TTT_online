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
pthread_t server_tid[128];

void *communication(void *);

void *server_main(void *arg) {
	struct server_data *server_data = (struct server_data *)arg;
	server_data->server_tid			= pthread_self();
	// creating socket and connecting to it
#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#elif defined(__linux__) || defined(ANDROID)
	struct sockaddr_in address;
	int server_fd, opt = 1, addrlen = sizeof(address);
#endif
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if ((server_fd < 0))
		return NULL;

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	memset(&address, 0, sizeof(struct sockaddr_in)); // safety
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port		= htons(server_data->PORT);
	// __android_log_print(ANDROID_LOG_VERBOSE, "Simple_TTT", "PORT: %i", address.sin_port);
	if (bind(server_fd, (const struct sockaddr *)&address, sizeof(struct sockaddr_in)))
		return NULL;

	// accepting clients
	listen(server_fd, 10);
	clientfd[0] = accept(server_fd, NULL, NULL); // also this fails on android because the port is not 5555
	read(clientfd[0], (char *)&server_data->data.users[1], sizeof(server_data->data.users[1]));

	listen(server_fd, 10);
	clientfd[1] = accept(server_fd, NULL, NULL);
	read(clientfd[1], (char *)&server_data->data.users[2], sizeof(server_data->data.users[2]));

	// initializing connection
	listen(server_fd, 10);
	write(clientfd[0], (char *)&server_data->data.users, sizeof(server_data->data.users));
	write(clientfd[0], (char *)&server_data->data.turn, 4);
	server_data->data.turn++;
	write(clientfd[1], (char *)&server_data->data.users, sizeof(server_data->data.users));
	write(clientfd[1], (char *)&server_data->data.turn, 4);

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
			end_server_game(server_data->data.winner, &server_data->data); // checks if all clients are ready to continue
	}
	close(clientfd[client_id]);
	pthread_exit(NULL);
}