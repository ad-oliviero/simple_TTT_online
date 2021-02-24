#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include <unistd.h>
#include "include/main.h"

extern int game_grid[9];
extern int is_game_over;
extern bool turn;
extern int winsP1;
extern int winsP2;

int checkwinner() {
	if ((game_grid[0] == game_grid[1]) && (game_grid[1] == game_grid[2])) {
		if (game_grid[0] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[0] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[3] == game_grid[4]) && (game_grid[4] == game_grid[5])) {
		if (game_grid[3] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[3] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[6] == game_grid[7]) && (game_grid[7] == game_grid[8])) {
		if (game_grid[6] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[6] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[0] == game_grid[3]) && (game_grid[3] == game_grid[6])) {
		if (game_grid[0] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[0] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[1] == game_grid[4]) && (game_grid[4] == game_grid[7])) {
		if (game_grid[1] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[1] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[2] == game_grid[5]) && (game_grid[5] == game_grid[8])) {
		if (game_grid[2] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[2] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[0] == game_grid[4]) && (game_grid[4] == game_grid[8])) {
		if (game_grid[0] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[0] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if ((game_grid[2] == game_grid[4]) && (game_grid[4] == game_grid[6])) {
		if (game_grid[2] == 1) {
			is_game_over = 1;
			return 1;
		} else if (game_grid[2] == 2) {
			is_game_over = 1;
			return 2;
		}
	} else if (game_grid[0] != 0 && game_grid[1] != 0 && game_grid[2] != 0 && game_grid[3] != 0 && game_grid[4] != 0 && game_grid[5] != 0 && game_grid[6] != 0 && game_grid[7] != 0 && game_grid[8] != 0) {
		is_game_over = 1;
		return 3;
	}
	return 0;
}

void endGame(int winner) {
	if (is_game_over) {
		for (int i = 0; i < 9; i ++) {
			game_grid[i] = 0;
		}
		if (winner == 1) {
			winsP1++;
			turn = !turn;
		} else if (winner == 2) {
			winsP2++;
			turn = !turn;
		}
		is_game_over = 0;
		turn = !turn;
		usleep(150000);
	}
}