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
#define MAXLINE 1000
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

	char buf[1000] = "";
	char *msg	   = "Client cciao\n\n";
	if (recv(*sockfd, buf, sizeof(buf), 0) < 0) {
		LOGE("Receive error, server could be offline.");
		exit(-1);
	}
	LOGI("Server says: %s", buf);
	if (send(*sockfd, "client Ciao!", strlen("client Ciao!"), 0) < 0) {
		LOGE("Send error, server could be offline.");
		exit(-1);
	}
	close(*sockfd);

	/* #ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
#endif
	struct sockaddr_in server_id;
	inet_pton(AF_INET, IP_ADDRESS, &server_id.sin_addr);
	server_id.sin_family = AF_INET;
	server_id.sin_port	 = htons(PORT);
	*sock				 = socket(AF_INET, SOCK_STREAM, 0);
	if (*sock < 0)
		return -1;
	return connect(*sock, (struct sockaddr *)&server_id, sizeof(server_id)); */
	return 0;
}

void *client_comm(void *arg) { // communicating data with the server (mostly receiving)
							   /* // initializing the game
							   struct client_data *data = (struct client_data *)arg;
							   // listen(data->sockfd, 10);
							   write(data->sockfd, (char *)&data->users[0], sizeof(data->users[0]));
							   read(data->sockfd, (char *)&data->users, sizeof(data->users));
							   read(data->sockfd, (char *)&data->user_id, sizeof(data->user_id));
							   if (data->user_id < 0) {
								   LOGE("user_id %d\n", data->user_id);
								   exit(-1);
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