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
	int user_id;
	char users[4][32];
	int bot_hardness;
	int game_mode;
	SOCK sock;
};

struct server_data {
	char IP_ADDRESS[16];
	int PORT;
	long int server_tid;
	int thread_id;
	long int client_tid[4];
	int client_running;
	struct client_data data;
};

void *window_main();

#endif // __MAIN_H__