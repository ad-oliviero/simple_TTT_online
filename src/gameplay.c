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
extern char user1[10];
extern char user2[10];

int checkwinner() {
	// columns
	for (int i = 0; i < 3; i++) {
		if (game_grid[i] == game_grid[i + 3] && game_grid[i + 3] == game_grid[i + 6] && game_grid[i] != 0) {
			is_game_over = 1;
			return game_grid[i];
		}
	}
	// raws
	for (int i = 0; i < 9; i += 3) {
		if (game_grid[i] == game_grid[i + 1] && game_grid[i + 1] == game_grid[i + 2] && game_grid[i] != 0) {
			is_game_over = 1;
			return game_grid[i];
		}
	}
	// diagonals
	if (game_grid[0] == game_grid[4] && game_grid[4] == game_grid[8] && game_grid[0] != 0 && game_grid[4] != 0 && game_grid[8] != 0) {
		is_game_over = 1;
		return game_grid[0];
	}
	if (game_grid[2] == game_grid[4] && game_grid[4] == game_grid[6] && game_grid[2] != 0 && game_grid[4] != 0 && game_grid[6] != 0) {
		is_game_over = 1;
		return game_grid[2];
	}
	// draw
	if (game_grid[0] != 0 && game_grid[1] != 0 && game_grid[2] != 0 && game_grid[3] != 0 && game_grid[4] != 0 && game_grid[5] != 0 && game_grid[6] != 0 && game_grid[7] != 0 && game_grid[8] != 0) {
		is_game_over = 1;
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
		DrawText(TextFormat("%s (X) won!", user1), MeasureText(TextFormat("%s (X) won!", user1), 20), SCR_HEIGHT / 2.7f, 20, BLACK);//SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	} else if (winner == 2) {
		DrawText(TextFormat("%s (O) won!", user2), MeasureText(TextFormat("%s (O) won!", user2), 20), SCR_HEIGHT / 2.7f, 20, BLACK);//SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	} else if (winner == 3) {
		DrawText("Draw...", SCR_WIDTH / 2.45f, SCR_HEIGHT / 2.74f, 30, BLACK);
	}

	if (CheckCollisionPointRec(GetMousePosition(), restart_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		ready = 1;
		usleep(150000);
	}
}
