#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#define SOCK int sock
#elif _WIN32
#include <winsock2.h>
#define SOCK unsigned int sock
#endif
#include "include/main.h"

extern int game_running;
extern struct online_data client_data;
extern char user0[32];
extern char user1[32];
extern char user_name[32];

SOCK;
int user_id = -1;
// char recv_msg[1024];
// char send_msg[128];

int client_connect(char *IP_ADDRESS, int PORT)
{ // connect to the sock
#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#endif
	struct sockaddr_in server_id;
	server_id.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_id.sin_family = AF_INET;
	server_id.sin_port = htons(PORT);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	return connect(sock, (struct sockaddr *)&server_id, sizeof(server_id));
}

void *client_comm(/* void *arg_data */)
{ // communicating data with the server (mostly receiving)
	// initializing the game
	// struct online_data *data = malloc(sizeof(data));
	// free(arg_data);
	listen(sock, 1);
	send(sock, (char *)&user_name, sizeof(user_name), 0);
	recv(sock, (char *)&user0, sizeof(user0), 0);
	recv(sock, (char *)&user1, sizeof(user1), 0);
	recv(sock, (char *)&user_id, 4, 0);

	// communication loop
	while (game_running)
	{
		// read game data
		recv(sock, (char *)&client_data.is_game_over, 4, 0);
		recv(sock, (char *)&client_data.turn, 4, 0);
		recv(sock, (char *)&client_data.winsP0, 4, 0);
		recv(sock, (char *)&client_data.winsP1, 4, 0);
		recv(sock, (char *)&client_data.winner, 4, 0);
		for (int i = 0; i < 9; i++)
			recv(sock, (char *)&client_data.game_grid[i], 4, 0);

		// write events
		if (client_data.turn == user_id)
			client_data.click_position = -1; // set click only if it's client's turn
		send(sock, (char *)&client_data.click_position, 4, 0);
		if (client_data.click_position >= 0 || client_data.is_game_over)
			client_data.click_position = -1;
		send(sock, (char *)&client_data.ready, 4, 0);
		if (client_data.ready == 1 && client_data.is_game_over == 0)
			client_data.ready = 0;
	}
	pthread_exit(NULL);
}