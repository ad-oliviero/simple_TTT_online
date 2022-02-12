package com.thedarkbug.Simple_TTT;

import android.view.inputmethod.InputMethodManager;
import android.content.Context;
import android.view.KeyEvent;
import android.app.Instrumentation;

public class NativeLoader extends android.app.NativeActivity {
    static {
        System.loadLibrary("Simple_TTT");      
    }
    
    public void showKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
        imm.toggleSoftInput(InputMethodManager.SHOW_IMPLICIT, 0);
    }

    public void hideKeyboard() {
        // send a back key event because the "normal" way with InputMethodManager does not work for some reason
        // I am not a java dev, and I apologize for this shitty code
        new Thread(new Runnable() { @Override public void run() { new Instrumentation().sendKeyDownUpSync(KeyEvent.KEYCODE_BACK); } }).start();
    }
}
