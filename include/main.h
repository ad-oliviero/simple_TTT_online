#ifndef __MAIN_H__
#define __MAIN_H__

#define SCR_WIDTH 450
#define SCR_HEIGHT 800
#define THICKNESS 3.0f

struct online_data
{
	int game_grid[9];
	int is_btn_pressed;
	int is_game_over;
	int click_position;
	int ready;
	int turn;
	int winsP0;
	int winsP1;
	int winner;
	int user_id;
	char users[4][32];
};

struct server_args
{
	char IP_ADDRESS[16];
	int PORT;
	long int server_tid;
	int thread_id;
	long int client_tid[4];
	int client_running;
	struct online_data data;
};

void *window_main();

#endif // __MAIN_H__