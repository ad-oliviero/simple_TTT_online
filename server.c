#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "include/gameplay.h"
#include "include/main.h"

int clientfd[2];
int ready_check[2];
struct online_data server_data = {0};
char user0[32];
char user1[32];

void *communication(void *);

int main(/* int argc, char **argv */)
{
	// creating socket and connecting to it
	struct sockaddr_in address;
	int server_fd, opt = 1, addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(5555);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return -1;

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
	pthread_t tid[2];
	for (int i = 0; i <= 1; i++)
	{
		int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&tid[i], 0, communication, arg);
	}
	for (int i = 0; i <= 1; i++)
		pthread_join(tid[i], NULL);
	return 0;
}

void *communication(void *arg)
{ // communicating server_data with the client (mostly sending)
	int i = *((int *)arg);
	free(arg);
	while (1)
	{
		// server_data.turn = !server_data.turn;
		server_data.winner = checkwinner();
		// send game server_data
		send(clientfd[i], (char *)&server_data.is_game_over, 4, 0);
		send(clientfd[i], (char *)&server_data.turn, 4, 0);
		send(clientfd[i], (char *)&server_data.winsP0, 4, 0);
		send(clientfd[i], (char *)&server_data.winsP1, 4, 0);
		send(clientfd[i], (char *)&server_data.winner, 4, 0);
		for (int j = 0; j < 9; j++)
			send(clientfd[i], &server_data.game_grid[j], 4, 0);
		// read events
		recv(clientfd[i], (char *)&server_data.click_position, sizeof(server_data.click_position), 0);
		recv(clientfd[i], (char *)&ready_check[i], sizeof(ready_check), 0);
		if (server_data.turn == i)
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
			end_server_game(server_data.winner); // checks if all clients are ready to continue
	}
	close(clientfd[i]);
	pthread_exit(NULL);
}