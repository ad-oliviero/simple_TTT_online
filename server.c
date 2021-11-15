#include <unistd.h>
#include <stdio.h>
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCK int sock
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define SOCK unsigned int sock
#endif
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "include/gameplay.h"
#include "include/main.h"

int clientfd[2];
int ready_check[2];
struct online_data server_data = {0};
extern int game_running;
extern char user0[32];
extern char user1[32];
pthread_t server_tid[128];

void *communication(void *);

void *server_main(void *PORT_arg)
{
	int PORT = *(int *)PORT_arg;
	// creating socket and connecting to it
#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#endif
	struct sockaddr_in address;
	int server_fd, opt = 1, addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return NULL;

	// accepting clients
	listen(server_fd, 3);
	clientfd[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	recv(clientfd[0], (char *)&user0, sizeof(user0), 0);
	printf("Connected %s\n", user0);

	listen(server_fd, 3);
	clientfd[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	recv(clientfd[1], (char *)&user1, sizeof(user1), 0);
	printf("Connected %s\n", user1);

	// initializing connection
	listen(server_fd, 3);
	send(clientfd[0], (char *)&user0, sizeof(user0), 0);
	send(clientfd[0], (char *)&user1, sizeof(user1), 0);
	send(clientfd[0], (char *)&server_data.turn, 4, 0);
	server_data.turn++;
	send(clientfd[1], (char *)&user0, sizeof(user0), 0);
	send(clientfd[1], (char *)&user1, sizeof(user1), 0);
	send(clientfd[1], (char *)&server_data.turn, 4, 0);

	// creating and joining threads
	for (int i = 0; i <= 1; i++)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&server_tid[i], 0, communication, arg);
	}
	for (int i = 0; i <= 1; i++)
		pthread_join(server_tid[i], NULL);
	return 0;
}

void *communication(void *arg)
{ // communicating server_data with the client (mostly sending)
	int client_id = *((int *)arg);
	while (game_running)
	{
		// server_data.turn = !server_data.turn;
		server_data.winner = checkwinner(&server_data);

		// send server_data to client
		send(clientfd[client_id], (const char *)&server_data.is_game_over, sizeof(server_data.is_game_over), 0);
		send(clientfd[client_id], (const char *)&server_data.turn, sizeof(server_data.turn), 0);
		send(clientfd[client_id], (const char *)&server_data.winsP0, sizeof(server_data.winsP0), 0);
		send(clientfd[client_id], (const char *)&server_data.winsP1, sizeof(server_data.winsP1), 0);
		send(clientfd[client_id], (const char *)&server_data.winner, sizeof(server_data.winner), 0);
		for (int i = 0; i < 9; i++)
			send(clientfd[client_id], (const char *)&server_data.game_grid[i], sizeof(server_data.game_grid[i]), 0);

		// read client events
		recv(clientfd[client_id], (char *)&server_data.click_position, sizeof(server_data.click_position), 0);
		recv(clientfd[client_id], (char *)&ready_check[client_id], sizeof(ready_check), 0);

		// check if client has permission to play
		if (server_data.turn == client_id)
			server_data.click_position = -1; // accepting click_position only from player's turn client
		if (server_data.click_position != -1 && server_data.game_grid[server_data.click_position] == 0 && server_data.is_game_over == 0)
		{ // handling click_positions
			if (server_data.turn)
				server_data.game_grid[server_data.click_position] = 1;
			else
				server_data.game_grid[server_data.click_position] = 2;
			server_data.turn = !server_data.turn;
		}
		if (ready_check[0] && ready_check[1])
			end_server_game(server_data.winner, &server_data); // checks if all clients are ready to continue
	}
	close(clientfd[client_id]);
	pthread_exit(NULL);
}