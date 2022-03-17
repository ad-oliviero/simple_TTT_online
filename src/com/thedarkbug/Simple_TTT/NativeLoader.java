package com.thedarkbug.Simple_TTT;

import android.view.inputmethod.InputMethodManager;
import android.content.Context;
import android.view.KeyEvent;
import android.app.Instrumentation;
import android.util.Log;
import java.net.*;
import java.io.*;

public class NativeLoader extends android.app.NativeActivity {
    private static ServerData servdata = new ServerData();
    public static Thread tid[] = new Thread[4];
    public static Boolean game_running = true;
    static {
        // System.loadLibrary("c++_shared");
        // System.loadLibrary("Simple_TTT");
    }

    public void showKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
        imm.toggleSoftInput(InputMethodManager.SHOW_IMPLICIT, 0);
    }

    public void hideKeyboard() {
        // send a back key event because the "normal" way with InputMethodManager does
        // not work for some reason
        // I am not a java dev, and I apologize for this shitty code
        new Thread(new Runnable() {
            @Override
            public void run() {
                new Instrumentation().sendKeyDownUpSync(KeyEvent.KEYCODE_BACK);
            }
        }).start();
    }

    public boolean getKeyboardState() {
        InputMethodManager imm = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
        return imm.isActive();
    }

    public void serverMain() throws IOException {
        ServerSocket servfd = new ServerSocket(5555);
        while (servdata.client_count < 2) {
            servdata.clifd[servdata.client_count] = servfd.accept();
            servdata.client_running = false;
            tid[servdata.client_count] = new Thread() {
                public void run() {
                    try {
                        communication();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            };
            tid[servdata.client_count].start();
            while (!servdata.client_running)
                ;
            servdata.client_count++;
        }
    }

    public static void communication() throws IOException {
        int client_id = servdata.client_count;
        DataOutputStream out = new DataOutputStream(servdata.clifd[client_id].getOutputStream());
        DataInputStream in = new DataInputStream(servdata.clifd[client_id].getInputStream());
        for (int i = 0; i < 32; i++)
            servdata.data.users[client_id][i] = in.readByte();
        out.writeInt(client_id);
        servdata.client_running = true;
        while (game_running) {
            servdata.data.winner = checkwinner();
            for (int i = 0; i < 4; i++)
                out.write(servdata.data.users[i]);
            out.writeInt(servdata.data.is_game_over);
            out.writeInt(servdata.data.turn);
            out.writeInt(servdata.data.winsP[0]);
            out.writeInt(servdata.data.winsP[1]);
            out.writeInt(servdata.data.winner);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    out.writeInt(servdata.data.game_grid[i][j]);
            servdata.data.click_position[0] = in.readInt();
            servdata.data.click_position[1] = in.readInt();
            if (servdata.data.turn == client_id) {
                servdata.data.click_position[0] = -1;
                servdata.data.click_position[1] = -1;
            }
            if (servdata.data.click_position[0] != -1 && servdata.data.click_position[1] != -1
                    && servdata.data.is_game_over == 0) // handling click_positions
                if (servdata.data.game_grid[servdata.data.click_position[0]][servdata.data.click_position[1]] == 0)
                    servdata.data.game_grid[servdata.data.click_position[0]][servdata.data.click_position[1]] = (servdata.data.turn = (~servdata.data.turn)
                            & 1) + 1;
            if (servdata.ready_check[0] == 1 && servdata.ready_check[1] == 1) {
                for (int i = 0; i < 9; i++)
                    servdata.data.game_grid[i / 3][i % 3] = 0;
                if (servdata.data.winner == 1)
                    servdata.data.winsP[0]++;
                else if (servdata.data.winner == 2)
                    servdata.data.winsP[1]++;
                servdata.data.is_game_over = 0;
            }
            servdata.ready_check[client_id] = in.readInt();
        }
    }

    public static int checkwinner() {
        for (int i = 0; i < 3; i++) {
            // raws
            if (servdata.data.game_grid[i][0] == servdata.data.game_grid[i][1]
                    && servdata.data.game_grid[i][1] == servdata.data.game_grid[i][2]
                    && servdata.data.game_grid[i][0] != 0) {
                servdata.data.is_game_over = 1;
                return servdata.data.game_grid[i][0];
            }
            // columns
            if (servdata.data.game_grid[0][i] == servdata.data.game_grid[1][i]
                    && servdata.data.game_grid[1][i] == servdata.data.game_grid[2][i]
                    && servdata.data.game_grid[0][i] != 0) {
                servdata.data.is_game_over = 1;
                return servdata.data.game_grid[0][i];
            }
        }

        // diagonals
        if (servdata.data.game_grid[0][0] == servdata.data.game_grid[1][1]
                && servdata.data.game_grid[1][1] == servdata.data.game_grid[2][2]
                && servdata.data.game_grid[0][0] != 0) {
            servdata.data.is_game_over = 1;
            return servdata.data.game_grid[0][0];
        } else if (servdata.data.game_grid[0][2] == servdata.data.game_grid[1][1]
                && servdata.data.game_grid[1][1] == servdata.data.game_grid[2][0]
                && servdata.data.game_grid[0][2] != 0) {
            servdata.data.is_game_over = 1;
            return servdata.data.game_grid[0][2];
        }

        // draw
        else if (servdata.data.game_grid[0][0] != 0 && servdata.data.game_grid[0][1] != 0
                && servdata.data.game_grid[0][2] != 0 && servdata.data.game_grid[1][0] != 0
                && servdata.data.game_grid[1][1] != 0 && servdata.data.game_grid[1][2] != 0
                && servdata.data.game_grid[2][0] != 0 && servdata.data.game_grid[2][1] != 0
                && servdata.data.game_grid[2][2] != 0) {
            servdata.data.is_game_over = 1;
            return 3;
        }
        return 0;
    }
}
