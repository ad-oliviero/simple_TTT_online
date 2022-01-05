#include "../lib/raylib/src/extras/raygui.h"
#include "../lib/raylib/src/raylib.h"
#include "include/gui.h"
#include "include/main.h"
#include <stdio.h>
#include <unistd.h>

void end_client_game(struct client_data *data) {
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, .8f)); // for obfuscation effect, idk how to use shaders or something
	Rectangle restart_btn = {SCR_WIDTH / 5, SCR_HEIGHT / 3, SCR_WIDTH / 5 * 3, SCR_HEIGHT / 10};
	data->ready			  = GuiToggle(restart_btn, data->winner == 3 ? "Draw..." : (data->winner == 1 ? TextFormat("%s (X) WON!", data->users[0]) : TextFormat("%s (0) WON!", data->users[1])), data->ready);
}

// check if someone wins, returns 0 if no one wins, 1 if player 1 (X) wins, 2 if player 2 (O) wins, 3 if it's a draw
int checkwinner(struct client_data *data) {
	for (int i = 0; i < 3; i++) {
		// raws
		if (data->game_grid[i][0] == data->game_grid[i][1] && data->game_grid[i][1] == data->game_grid[i][2] && data->game_grid[i][0] != 0) {
			data->is_game_over = 1;
			return data->game_grid[i][0];
		}
		// columns
		if (data->game_grid[0][i] == data->game_grid[1][i] && data->game_grid[1][i] == data->game_grid[2][i] && data->game_grid[0][i] != 0) {
			data->is_game_over = 1;
			return data->game_grid[0][i];
		}
	}

	// diagonals
	if (data->game_grid[0][0] == data->game_grid[1][1] && data->game_grid[1][1] == data->game_grid[2][2] && data->game_grid[0][0] != 0) {
		data->is_game_over = 1;
		return data->game_grid[0][0];
	} else if (data->game_grid[0][2] == data->game_grid[1][1] && data->game_grid[1][1] == data->game_grid[2][0] && data->game_grid[0][2] != 0) {
		data->is_game_over = 1;
		return data->game_grid[0][2];
	}

	// draw
	else if (data->game_grid[0][0] != 0 && data->game_grid[0][1] != 0 && data->game_grid[0][2] != 0 && data->game_grid[1][0] != 0 && data->game_grid[1][1] != 0 && data->game_grid[1][2] != 0 && data->game_grid[2][0] != 0 && data->game_grid[2][1] != 0 && data->game_grid[2][2] != 0) {
		data->is_game_over = 1;
		return 3;
	}
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