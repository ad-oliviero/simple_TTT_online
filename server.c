#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "include/gameplay.h"
#include "include/main.h"

int click = -1;
int clientfd[2];
int is_game_over = 0;
int game_grid[9] = {0};
int ready_check[2];
int turn = 0;
int winsP0 = 0;
int winsP1 = 0;
int winner = 0;
char user0[USERN_LENGTH];
char user1[USERN_LENGTH];

void* communication(void*);

int main(void) {
	// creating socket and connecting to it
	struct sockaddr_in address;
	int server_fd, opt = 1, addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) return -1;

	// accepting clients
	listen(server_fd, 3);
	clientfd[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	recv(clientfd[0], (char *) &user0, sizeof(user0), 0);
	printf("Connected %s\n", user0);

	listen(server_fd, 3);
	clientfd[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	recv(clientfd[1], (char *) &user1, sizeof(user1), 0);
	printf("Connected %s\n", user1);

	// initializing connection
	listen(server_fd, 3);
	send(clientfd[0], (char *) &user0, sizeof(user0), 0);
	send(clientfd[0], (char *) &user1, sizeof(user1), 0);
	send(clientfd[0], (char *) &turn, 4, 0);
	turn++;
	send(clientfd[1], (char *) &user0, sizeof(user0), 0);
	send(clientfd[1], (char *) &user1, sizeof(user1), 0);
	send(clientfd[1], (char *) &turn, 4, 0);

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

void* communication(void* arg) {	// communicating data with the client (mostly sending)
	int i = *((int *) arg);
	free(arg);
	while (1) {
		// send game data
		send(clientfd[i], (char *) &is_game_over, 4, 0);
		send(clientfd[i], (char *) &turn, 4, 0);
		send(clientfd[i], (char *) &winsP0, 4, 0);
		send(clientfd[i], (char *) &winsP1, 4, 0);
		send(clientfd[i], (char *) &winner, 4, 0);
		for (int j = 0; j < 9; j++) {
			send(clientfd[i], &game_grid[j], 4, 0);
		}
		// read events
		recv(clientfd[i], (char *) &click, 4, 0);
		recv(clientfd[i], (char *) &ready_check[i], sizeof(ready_check), 0);
		winner = checkwinner();
		if (turn == i) click = -1; // accepting click only from player's turn client
		if (click != -1 && game_grid[click] == 0 && is_game_over == 0 ) { // handling clicks
			if (turn) game_grid[click] = 1;
			else game_grid[click] = 2;
			turn = !turn;
		}
		if (ready_check[0] && ready_check[1]) endGame(winner); // checks if all clients are ready to continue
	}
	close(clientfd[i]);
	pthread_exit(NULL);
}