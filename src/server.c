#include <stdio.h>
#include <unistd.h>
#if defined(__linux__) || defined(__EMSCRIPTEN__)
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/tcp.h>
#elif _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif
#include "include/gameplay.h"
#include "include/main.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef __ANDROID_API__
	#include <android_native_app_glue.h>
	#include <jni.h>
#endif

extern int game_running;

void *communication(void *arg) { // communicating servdata->data with the client (mostly sending)
#ifndef __EMSCRIPTEN__
	struct server_data *servdata = (struct server_data *)arg;
	int client_id				 = servdata->client_count;
	servdata->client_running	 = 1;
	int recv_flags				 = 0;
	int send_flags				 = 0;
	while (game_running) {
		servdata->data.winner = checkwinner(&servdata->data);
		for (int i = 0; i < 4; i++)
			send(servdata->clifd[client_id], &(servdata->data.users[i]), sizeof(servdata->data.users[i]), send_flags);
		int ibuf = 0;
		// send data in network byte order
		ibuf = htonl(servdata->data.is_game_over);
		send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
		ibuf = htonl(servdata->data.turn);
		send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
		ibuf = htonl(servdata->data.winsP[0]);
		send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
		ibuf = htonl(servdata->data.winsP[1]);
		send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
		ibuf = htonl(servdata->data.winner);
		send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				ibuf = htonl(servdata->data.game_grid[i][j]);
				send(servdata->clifd[client_id], &ibuf, sizeof(ibuf), send_flags);
			}

		// recv client events
		recv(servdata->clifd[client_id], &ibuf, sizeof(ibuf), recv_flags);
		servdata->data.click_position[0] = ntohl(ibuf);
		recv(servdata->clifd[client_id], &ibuf, sizeof(ibuf), recv_flags);
		servdata->data.click_position[1] = ntohl(ibuf);
		recv(servdata->clifd[client_id], &ibuf, sizeof(ibuf), recv_flags);
		servdata->ready_check[client_id] = ntohl(ibuf);

		// check if client has permission to play
		if (servdata->data.turn == client_id) {
			servdata->data.click_position[0] = -1;
			servdata->data.click_position[1] = -1;
		}
		// accepting click_position only from player's turn client
		if (servdata->data.click_position[0] != -1 &&
			servdata->data.click_position[1] != -1 &&
			servdata->data.is_game_over == 0)																	   // handling click_positions
			if (servdata->data.game_grid[servdata->data.click_position[0]][servdata->data.click_position[1]] == 0) // if positions exist, then check for the position being free
				servdata->data.game_grid[servdata->data.click_position[0]][servdata->data.click_position[1]] = (servdata->data.turn = !servdata->data.turn) + 1;
		if (servdata->ready_check[0] && servdata->ready_check[1])
			end_server_game(servdata->data.winner, &servdata->data); // checks if all clients are recvy to continue
	}
	close(servdata->clifd[client_id]);
#endif
	return NULL;
}

void *server_main(void *arg) {
#ifndef __EMSCRIPTEN__
	struct server_data *servdata = (struct server_data *)arg;
	pthread_t tid[128];
	// creating socket and connecting to it
	#ifdef _WIN32
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
	#endif
	struct sockaddr_in address;
	int flags = 1, addrlen = sizeof(address);
	SOCK servfd = socket(AF_INET, SOCK_STREAM, 6); // 6 = tcp protocol
	setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
	// setsockopt(servfd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags));
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port		= htons(servdata->PORT);
	if (bind(servfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		return NULL;
	listen(servfd, 10);

	// accepting clients and creating and joining threads
	servdata->client_count = 0;
	while (servdata->client_count < 2) {
		servdata->clifd[servdata->client_count] = accept(servfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		for (int i = 0; i < 32; i++)
			recv(servdata->clifd[servdata->client_count], &servdata->data.users[servdata->client_count][i], sizeof(servdata->data.users[servdata->client_count][i]), 0);
		int ibuf = htonl(servdata->client_count);
		send(servdata->clifd[servdata->client_count], &ibuf, sizeof(ibuf), 0);
		servdata->client_running = 0;
		pthread_create(&tid[servdata->client_count], 0, communication, servdata);
		while (servdata->client_running == 0)
			;
		servdata->client_count++;
	}
	for (int i = 0; i <= 1; i++)
		pthread_join(tid[i], NULL);
#endif
	return 0;
}
