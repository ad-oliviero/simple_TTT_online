#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "include/gameplay.h"
#define PORT 5555

int is_game_over = 0;
int turn = 1;
int winsP1 = 0;
int winsP2 = 0;
int winner = 0;
int game_grid[9] = {0};
int click = -1;
int ready_check[2];
char user1[10];
char user2[10];
int clientfd[2];

void* communication(void*);

int main(void) {
	// creating socket and connecting to it
	struct sockaddr_in address;
	int server_fd, opt = 1, addrlen = sizeof(address);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) exit(EXIT_FAILURE);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) exit(EXIT_FAILURE);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) exit(EXIT_FAILURE);

	// accepting clients
	if (listen(server_fd, 3) < 0) exit(EXIT_FAILURE);
	if ((clientfd[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) exit(EXIT_FAILURE);
	recv(clientfd[0], (char *) &user1, 10, 0);
	printf("Connected %s\n", user1);

	if (listen(server_fd, 3) < 0) exit(EXIT_FAILURE);
	if ((clientfd[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) exit(EXIT_FAILURE);
	recv(clientfd[1], (char *) &user2, 10, 0);
	printf("Connected %s\n", user2);

	// initializing connection
	if (listen(server_fd, 3) < 0) exit(EXIT_FAILURE);
	send(clientfd[0], (char *) &user1, 10, 0);
	send(clientfd[0], (char *) &user2, 10, 0);
	send(clientfd[1], (char *) &user1, 10, 0);
	send(clientfd[1], (char *) &user2, 10, 0);

	// creating and joining threads
	pthread_t tid[2];
	for (int i = 0; i <= 1; i++) {
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&tid[i], 0, communication, arg);
	}
	for (int i = 0; i <= 1; i++) pthread_join(tid[i], NULL);
	return 0;
}

void* communication(void* arg) {
	int i = *((int *) arg);
	free(arg);
	while (1) {
		// send game data
		send(clientfd[i], (char *) &is_game_over, 4, 0);
		send(clientfd[i], (char *) &turn, 4, 0);
		send(clientfd[i], (char *) &winsP1, 4, 0);
		send(clientfd[i], (char *) &winsP2, 4, 0);
		send(clientfd[i], (char *) &winner, 4, 0);
		for (int j = 0; j < 9; j++) {
			send(clientfd[i], &game_grid[j], 4, 0);
		}
		// read events
		recv(clientfd[i], (char *) &click, 4, 0);
		recv(clientfd[i], (char *) &ready_check[i], sizeof(ready_check), 0);
		winner = checkwinner();
		if (turn != i) click = -1; // accepting click only from player's turn client
		if (click != -1 && game_grid[click] == 0 && is_game_over == 0 ) { // handling clicks
			if (turn == 0) {
				game_grid[click] = 2;
				turn = 1;
			} else if (turn == 1) {
				game_grid[click] = 1;
				turn = 0;
			}
		}
		if (ready_check[0] == 1 && ready_check[1] == 1) endGame(winner); // checks if all clients are ready to continue
	}
	pthread_exit(NULL);
}