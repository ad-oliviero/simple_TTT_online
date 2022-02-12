#include "include/main.h"
#include "../lib/raylib/src/raylib.h"
#include "include/bot.h"
#include "include/client.h"
#include "include/gameplay.h"
#include "include/gui.h"
#include "include/server.h"
#include "include/shapes.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __ANDROID_API__
	#include <jni.h>
struct android_app *app;
#endif

int game_running = 0;

char *get_ip_addr(int id) {
	char *ip = NULL;
	if (id == 0) { // local ip
#ifndef __ANDROID_API__
		char hostbuffer[256];
		struct hostent *host_entry;
		int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
		if (hostname != -1) {
			host_entry = gethostbyname(hostbuffer);
			if (host_entry)
				ip = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
		}
#else
		FILE *ip_fp = popen("ifconfig $(getprop wifi.interface) | grep -Eo 'addr:[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}' | sed \"s/addr://g\"", "r");
		if (ip_fp == NULL) {
			LOGE("Failed to get external ip");
			return NULL;
		}
		ip = calloc(1, 48);
		fgets(ip, 48, ip_fp);
		fclose(ip_fp);
		// for some reason, on android a \n gets added
		char *ipp = strchr(ip, '\n');
		while (ipp) {
			*ipp = ' ';
			ipp	 = strchr(ipp, '\n');
		};
#endif
	} else { // external ip
		FILE *ip_fp = popen("curl -s ipinfo.io/ip", "r");
		if (ip_fp == NULL) {
			LOGE("Failed to get external ip");
			return NULL;
		}
		ip = calloc(1, 48);
		fgets(ip, 48, ip_fp);
		fclose(ip_fp);
	}
	return ip;
}

void init_game(struct client_data *data, struct server_data *server) {
	// resetting game data
#ifdef __ANDROID_API__
	SCR_WIDTH  = GetScreenWidth();
	SCR_HEIGHT = GetScreenHeight();
#endif
	data->click_position[0] = -1;
	data->click_position[1] = -1;
	data->uid				= -1;
	data->local_ip			= calloc(1, 128);
	server->PORT			= 5555;
	data->game_mode			= join_window(server->IP_ADDRESS, &server->PORT, data);
#ifdef __ANDROID_API__
	toggleKeyboard(false);
#endif
	pthread_t tid[4];
	if (data->game_mode < 0) {
		LOGE("Failed to join game");
		exit(1);
	} else if (data->game_mode == 1 || data->game_mode == 2) {
		pthread_create(&tid[2], 0, server_main, server);
		while (client_connect(server->IP_ADDRESS, server->PORT, &data->sockfd))
			;
	}
	if (data->game_mode == 2)
		sprintf(data->username, "Me");
	pthread_create(&tid[0], 0, client_comm, data);
	if (data->game_mode == 2)
		pthread_create(&tid[3], 0, bot_main, NULL);
	else {
		// get addresses
		char *local_ip	  = get_ip_addr(0);
		char *external_ip = get_ip_addr(1);
		if (local_ip != NULL) {
			sprintf(data->local_ip, "IP: %s", local_ip);
			if (external_ip)
				sprintf(data->local_ip, "%s / %s", data->local_ip, external_ip);
		} else {
			if (external_ip)
				sprintf(data->local_ip, "IP: %s", external_ip);
		}
		free(external_ip);
#ifdef __ANDROID_API__
		free(local_ip);
#endif
	}
}

void main_window(struct client_data *data) {
	// main window
	initHitBox();
	SetWindowTitle(TextFormat("Simple TTT - %s", data->username));
	while (!WindowShouldClose() && game_running) {
		place(data);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				shape((int[2]){i, j}, &data->game_grid[i][j]);
		if (data->is_game_over == 1)
			end_client_game(data);
		matchInfo(data);
		EndDrawing();
	}
}

void sigpipe_handler(int signum) {
	LOGE("Client disconnected: %d", signum);
	exit(1);
}

int main() {
	signal(SIGPIPE, sigpipe_handler);
#ifdef __ANDROID_API__
	app = GetAndroidApp();
#endif
	struct client_data *data   = calloc(1, sizeof(struct client_data));
	struct server_data *server = calloc(1, sizeof(struct server_data));
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCR_WIDTH, SCR_HEIGHT, "Game Mode Selection");
	SetTargetFPS(GetMonitorRefreshRate(0));
	init_game(data, server);
	main_window(data);
	// end of the program
	game_running = 0;
	CloseWindow();
	close(data->sockfd);
	return 0;
}
