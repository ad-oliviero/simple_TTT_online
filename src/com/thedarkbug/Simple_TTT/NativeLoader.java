package com.thedarkbug.Simple_TTT;
public class NativeLoader extends android.app.NativeActivity {
    static {
        System.loadLibrary("Simple_TTT"); // must match name of shared library (in this case libSimple_TTT.so)        
    }
}
