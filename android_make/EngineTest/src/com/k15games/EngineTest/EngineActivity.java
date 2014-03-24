package com.k15games.EngineTest;
import android.app.NativeActivity;
import android.util.Log;

public class EngineActivity extends NativeActivity  {

	private static final String TAG = "EngineActivity";

    static {
        System.loadLibrary("Engine");
    }

    public EngineActivity() {
    	super();
    	Log.v(TAG,"Starting EngineActivity...");
    }
}