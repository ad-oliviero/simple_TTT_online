#ifndef __MAIN_H__
#define __MAIN_H__

#define SCR_WIDTH 450
#define SCR_HEIGHT 800
#define THICKNESS 3.0f
#define BLOCK SCR_WIDTH / 3
#ifdef _WIN32
	#define SOCK unsigned int
#else
	#define SOCK int
#endif

struct client_data {
	int game_grid[3][3];
	int is_btn_pressed;
	int is_game_over;
	int click_position[2];
	int ready;
	int turn;
	int winsP[2];
	int winner;
	int uid;
	char users[4][32];
	char username[32];
	int game_mode;
	SOCK sockfd;
};

struct server_data {
	char IP_ADDRESS[16];
	int PORT;
	int client_count;
	int client_running;
	int ready_check[4];
	SOCK clifd[4];
	struct client_data data;
};

void *window_main();

#endif // __MAIN_H__