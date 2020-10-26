package com.temple.shaderimage;

import android.os.Handler;
import android.os.Looper;

public class Utils {

    private static final Handler mainHandler = new Handler(Looper.getMainLooper());

    public static void post(Runnable runnable) {
        mainHandler.post(runnable);
    }
}
