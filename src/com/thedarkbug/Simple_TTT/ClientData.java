package com.thedarkbug.Simple_TTT;

import java.net.Socket;

public class ClientData {
	public int[][] game_grid;
	public int is_btn_pressed;
	public int is_game_over;
	public int[] click_position;
	public int ready;
	public int turn;
	public int[] winsP;
	public int winner;
	public int uid;
	public byte[][] users;
	public String username;
	public int game_mode;
	public String local_ip;
	public Socket sockfd;

	ClientData() {
		this.game_grid = new int[3][3];
		this.click_position = new int[2];
		this.click_position[0] = -1;
		this.click_position[1] = -1;
		this.winsP = new int[2];
		this.users = new byte[4][32];
	}
}
