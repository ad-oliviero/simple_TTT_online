package com.thedarkbug.Simple_TTT;

import java.net.Socket;

public class ServerData {
	public String IP_ADDRESS;
	public int PORT;
	public int client_count;
	public Boolean client_running;
	public int[] ready_check;
	public Socket[] clifd;
	public ClientData data;

	ServerData() {
		this.PORT = 5555;
		this.client_count = 0;
		this.clifd = new Socket[4];
		this.ready_check = new int[4];
		this.data = new ClientData();
	}
}
