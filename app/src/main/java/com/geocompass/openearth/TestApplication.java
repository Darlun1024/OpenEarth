package com.geocompass.openearth;

import android.app.Application;

import com.geocompass.openearth.sdk.OpenEarth;
import com.squareup.leakcanary.LeakCanary;

/**
 * Created by gxsn on 2017/12/6.
 */

public class TestApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        super.onCreate();
        OpenEarth.init();
        if (LeakCanary.isInAnalyzerProcess(this)) {
            // This process is dedicated to LeakCanary for heap analysis.
            // You should not init your app in this process.
            return;
        }
        LeakCanary.install(this);
        // Normal app init code...
    }
}
