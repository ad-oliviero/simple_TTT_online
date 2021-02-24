#ifdef __linux__
	#include <raylib.h>
#elif _WIN32
	#include "include/raylib.h"
#endif
#include <unistd.h>
#include "include/main.h"

extern int game_grid[9];
extern int is_game_over;
extern int ready;
extern bool turn;
extern int winsP1;
extern int winsP2;
extern int winner;

int checkwinner() {
	if ((game_grid[0] == game_grid[1]) && (game_grid[1] == game_grid[2])) {
		if (game_grid[0] == 1) {
			return 1;
		} else if (game_grid[0] == 2) {
			return 2;
		}
	} else if ((game_grid[3] == game_grid[4]) && (game_grid[4] == game_grid[5])) {
		if (game_grid[3] == 1) {
			return 1;
		} else if (game_grid[3] == 2) {
			return 2;
		}
	} else if ((game_grid[6] == game_grid[7]) && (game_grid[7] == game_grid[8])) {
		if (game_grid[6] == 1) {
			return 1;
		} else if (game_grid[6] == 2) {
			return 2;
		}
	} else if ((game_grid[0] == game_grid[3]) && (game_grid[3] == game_grid[6])) {
		if (game_grid[0] == 1) {
			return 1;
		} else if (game_grid[0] == 2) {
			return 2;
		}
	} else if ((game_grid[1] == game_grid[4]) && (game_grid[4] == game_grid[7])) {
		if (game_grid[1] == 1) {
			return 1;
		} else if (game_grid[1] == 2) {
			return 2;
		}
	} else if ((game_grid[2] == game_grid[5]) && (game_grid[5] == game_grid[8])) {
		if (game_grid[2] == 1) {
			return 1;
		} else if (game_grid[2] == 2) {
			return 2;
		}
	} else if ((game_grid[0] == game_grid[4]) && (game_grid[4] == game_grid[8])) {
		if (game_grid[0] == 1) {
			return 1;
		} else if (game_grid[0] == 2) {
			return 2;
		}
	} else if ((game_grid[2] == game_grid[4]) && (game_grid[4] == game_grid[6])) {
		if (game_grid[2] == 1) {
			return 1;
		} else if (game_grid[2] == 2) {
			return 2;
		}
	} else if (game_grid[0] != 0 && game_grid[1] != 0 && game_grid[2] != 0 && game_grid[3] != 0 && game_grid[4] != 0 && game_grid[5] != 0 && game_grid[6] != 0 && game_grid[7] != 0 && game_grid[8] != 0) {
		return 3;
	}
	return 0;
}

void endGame() {
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, .5f));
	Rectangle restart_btn = {SCR_WIDTH / 5, SCR_HEIGHT / 3, SCR_WIDTH / 5 * 3, SCR_HEIGHT / 10};
	DrawRectangleRounded(restart_btn, .7f, 20, DARKGRAY);
	DrawRectangleRoundedLines(restart_btn, .7f, 100, 5, RED);

	if (winner == 1) {
		DrawText("Player 1 (X) WON!", SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	} else if (winner == 2) {
		DrawText("Player 2 (O) WON!", SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	} else if (winner == 3) {
		DrawText("Draw...", SCR_WIDTH / 2.45f, SCR_HEIGHT / 2.74f, 30, BLACK);
	}

	if (CheckCollisionPointRec(GetMousePosition(), restart_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		ready = 1;
		usleep(150000);
	}
}