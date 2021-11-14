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
};

void *window_main();

#endif // __MAIN_H__