#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "include/gameplay.h"
#define PORT 5555

int is_game_over = 0;
int turn = 1;
int winsP1 = 0;
int winsP2 = 0;
int winner = 0;
int game_grid[9] = {
	0, 0, 0,
	0, 0, 0,
	0, 0, 0
};
int click = -1;
int ready_check[2];
int permisson[2];

int main() {
	struct sockaddr_in address;
	int server_fd, new_socket[2], opt = 1, addrlen = sizeof(address), click = 1;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) exit(EXIT_FAILURE);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) exit(EXIT_FAILURE);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) exit(EXIT_FAILURE);
	if (listen(server_fd, 3) < 0) exit(EXIT_FAILURE);
	if ((new_socket[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) exit(EXIT_FAILURE);
	printf("Connecten P1!\n");
	
	if (listen(server_fd, 3) < 0) exit(EXIT_FAILURE);
	if ((new_socket[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) exit(EXIT_FAILURE);
	printf("Connecten P2!\n");
	permisson[0] = 0;
	permisson[1] = 1;
	while (1) {

		if (checkwinner() != 0) {
			is_game_over = 1;
			winner = checkwinner();
		}
		//send game data
		for (int i = 0; i <= 1; i++) {
			send(new_socket[i], &is_game_over, 4, 0);
			send(new_socket[i], &turn, 4, 0);
			send(new_socket[i], &winsP1, 4, 0);
			send(new_socket[i], &winsP2, 4, 0);
			send(new_socket[i], &winner, 4, 0);
			for (int j = 0; j < 9; j++) {
				send(new_socket[i], &game_grid[j], 4, 0);
			}

			// read events
			recv(new_socket[i], &click, 4, 0);
			recv(new_socket[i], &ready_check[i], sizeof(ready_check), 0);
			
			if (click != -1 && game_grid[click] == 0 && is_game_over == 0 ) {
				if (turn == 0) {
					game_grid[click] = 2;
					turn = 1;
				} else if (turn == 1) {
					game_grid[click] = 1;
					turn = 0;
				}
			}
		}
		if (ready_check[0] == 1 && ready_check[1] == 1) endGame(winner);
	}
	return 0;
}
