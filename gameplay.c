#include "lib/raylib/include/raylib.h"
#include "lib/raygui/src/raygui.h"
#include <unistd.h>
#include <stdio.h>
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

int checkwinner()
{ // check if someone wins
	// columns
	for (int i = 0; i < 3; i++)
	{
		if (game_grid[i] == game_grid[i + 3] && game_grid[i + 3] == game_grid[i + 6] && game_grid[i] != 0)
		{
			is_game_over = 1;
			return game_grid[i];
		}
	}
	// raws
	for (int i = 0; i < 9; i += 3)
	{
		if (game_grid[i] == game_grid[i + 1] && game_grid[i + 1] == game_grid[i + 2] && game_grid[i] != 0)
		{
			is_game_over = 1;
			return game_grid[i];
		}
	}
	// diagonals
	if (game_grid[0] == game_grid[4] && game_grid[4] == game_grid[8] && game_grid[0] != 0 && game_grid[4] != 0 && game_grid[8] != 0)
	{
		is_game_over = 1;
		return game_grid[0];
	}
	if (game_grid[2] == game_grid[4] && game_grid[4] == game_grid[6] && game_grid[2] != 0 && game_grid[4] != 0 && game_grid[6] != 0)
	{
		is_game_over = 1;
		return game_grid[2];
	}
	// draw
	if (game_grid[0] != 0 && game_grid[1] != 0 && game_grid[2] != 0 && game_grid[3] != 0 && game_grid[4] != 0 && game_grid[5] != 0 && game_grid[6] != 0 && game_grid[7] != 0 && game_grid[8] != 0)
	{
		is_game_over = 1;
		return 3;
	}
	return 0;
}

void endGame()
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, .8f)); // for obfuscation effect, idk how to use shaders or something
	Rectangle restart_btn = {SCR_WIDTH / 5, SCR_HEIGHT / 3, SCR_WIDTH / 5 * 3, SCR_HEIGHT / 10};
	char restart_text[32] = "Draw...";

	// write text in the button, needs some fixes for big nicknames
	if (winner == 1)
		sprintf(restart_text, "Player 1 (X) WON!");
	else if (winner == 2)
		sprintf(restart_text, "Player 2 (O) WON!");

	if (GuiToggle(restart_btn, restart_text, ready))
		ready = 1;
}
