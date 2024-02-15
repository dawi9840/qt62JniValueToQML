package com.foxconn;

import android.content.Context;
import android.content.ComponentName;
import android.content.Intent;
import android.util.Log;
import android.app.Service;
import android.os.IBinder;


public class QtAndroidService extends Service{
    private static final String TAG = "QtAndroidService";

    // Native methods for sending different types of messages to Qt
    private static native void sendToQt(String message);

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "onCreate");

        String msg = "Hello_world_from_c++";
        testSendToQt(10, msg);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        //Log.i(TAG, "Destroying Service");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void testSendToQt(int count, String msg) {
        Log.d(TAG, "testSendToQt");
        new Thread(() -> {
            for(int i=0; i<=count; i++){
                if (msg!=null && !msg.isEmpty()) {
                    Log.d(TAG, "java(count, msg): " + i + ", " + msg);
                    sendToQt(msg);
                }
            }
        }).start();
    }
}
