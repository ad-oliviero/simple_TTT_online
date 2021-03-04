#include "include/raylib.h"
#include <unistd.h>
#include "include/main.h"

extern int game_grid[9];
extern int is_game_over;
extern int ready;
extern int turn;
extern int winsP0;
extern int winsP1;
extern int winner;
extern char user0[USERN_LENGTH];
extern char user1[USERN_LENGTH];

int checkwinner() {	// check if someone wins
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

void endGame() {	// button to continue the game after a match ends
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, .5f));	// for obfuscation effect, idk how to use shaders or something
	// drawing actual button
	Rectangle restart_btn = {SCR_WIDTH / 5, SCR_HEIGHT / 3, SCR_WIDTH / 5 * 3, SCR_HEIGHT / 10};
	DrawRectangleRounded(restart_btn, .7f, 20, DARKGRAY);
	DrawRectangleRoundedLines(restart_btn, .7f, 100, 5, RED);

	// write text in the button, needs some fixes for big nicknames
	if (winner == 1) DrawText(TextFormat("%s (X) won!", user0), MeasureText("(X) won!", 20) + restart_btn.x, SCR_HEIGHT / 2.7f, 20, BLACK); //SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	else if (winner == 2) DrawText(TextFormat("%s (O) won!", user1), MeasureText("(O) won!", 20) + restart_btn.x, SCR_HEIGHT / 2.7f, 20, BLACK); //SCR_WIDTH / 3.3f, SCR_HEIGHT / 2.7f, 20, BLACK);
	else if (winner == 3) DrawText("Draw...", SCR_WIDTH / 2.45f, SCR_HEIGHT / 2.74f, 30, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), restart_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		ready = 1;
		usleep(150000);
	}
}
