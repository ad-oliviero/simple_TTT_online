#ifndef __SERVER__
	#include "lib/raygui/src/raygui.h"
	#include "lib/raylib/include/raylib.h"
#endif // __SERVER__
#include "include/main.h"
#include <stdio.h>
#include <unistd.h>

#ifndef __SERVER__
extern char user0[32];
#else  // __SERVER__
extern struct client_data server_data;
#endif // __SERVER__

void end_client_game(struct client_data *data) {
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, .8f)); // for obfuscation effect, idk how to use shaders or something
	Rectangle restart_btn = {SCR_WIDTH / 5, SCR_HEIGHT / 3, SCR_WIDTH / 5 * 3, SCR_HEIGHT / 10};

	if (GuiToggle(restart_btn, data->winner == 3 ? "Draw..." : (data->winner == 1 ? TextFormat("%s (X) WON!", data->users[1]) : TextFormat("%s (0) WON!", data->users[2])), data->ready))
		data->ready = 1;
}

int checkwinner(struct client_data *data) { // check if someone wins
	if (data->game_grid[0][0] != 0 && data->game_grid[0][1] != 0 && data->game_grid[0][2] != 0 && data->game_grid[1][0] != 0 && data->game_grid[1][1] != 0 && data->game_grid[1][2] != 0 && data->game_grid[2][0] != 0 && data->game_grid[2][1] != 0 && data->game_grid[2][2] != 0) {
		data->is_game_over = 1;
		return 3;
	}

	// columns
	/* for (int i = 0; i < 3; i++) {
		if (data->game_grid[i] == data->game_grid[i + 3] && data->game_grid[i + 3] == data->game_grid[i + 6] && data->game_grid[i] != 0) {
			data->is_game_over = 1;
			return data->game_grid[i];
		}
	}
	// raws
	for (int i = 0; i < 9; i += 3) {
		if (data->game_grid[i] == data->game_grid[i + 1] && data->game_grid[i + 1] == data->game_grid[i + 2] && data->game_grid[i] != 0) {
			data->is_game_over = 1;
			return data->game_grid[i];
		}
	}
	// diagonals
	if (data->game_grid[0] == data->game_grid[4] && data->game_grid[4] == data->game_grid[8] && data->game_grid[0] != 0 && data->game_grid[4] != 0 && data->game_grid[8] != 0) {
		data->is_game_over = 1;
		return data->game_grid[0];
	}
	if (data->game_grid[2] == data->game_grid[4] && data->game_grid[4] == data->game_grid[6] && data->game_grid[2] != 0 && data->game_grid[4] != 0 && data->game_grid[6] != 0) {
		data->is_game_over = 1;
		return data->game_grid[2];
	}

	// draw
	if (data->game_grid[0] != 0 && data->game_grid[1] != 0 && data->game_grid[2] != 0 && data->game_grid[3] != 0 && data->game_grid[4] != 0 && data->game_grid[5] != 0 && data->game_grid[6] != 0 && data->game_grid[7] != 0 && data->game_grid[8] != 0) {
		data->is_game_over = 1;
		return 3;
	} */
	return 0;
}

void end_server_game(int winner, struct client_data *data) { // resetting the game and update variables
	for (int i = 0; i < 9; i++)
		data->game_grid[i / 3][i % 3] = 0;
	if (winner == 1)
		data->winsP[0]++;
	else if (winner == 2)
		data->winsP[1]++;
	data->is_game_over = 0;
}
