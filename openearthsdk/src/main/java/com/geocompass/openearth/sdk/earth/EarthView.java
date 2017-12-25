package com.geocompass.openearth.sdk.earth;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

import com.geocompass.openearth.sdk.R;


/**
 * Created by gxsn on 2017/11/13.
 */

public class EarthView extends FrameLayout implements View.OnGenericMotionListener{

    private GLSurfaceView mSurfaceView;
    private Earth mEarth;


    public EarthView(Context context, AttributeSet attrs) {
        super(context, attrs);
        if(isSupportES2())
            initialize();
    }


    private void initialize() {
        //初始化SurfaceView
        View view = LayoutInflater.from(getContext()).inflate(R.layout.earth_view, this);
        mSurfaceView = view.findViewById(R.id.surface_view);
        mEarth = new Earth(mSurfaceView);

        setClickable(true);
        setLongClickable(true);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestDisallowInterceptTouchEvent(true);
    }


    /**
     * 检查设备使用支持 OpenGLES 2
     *
     * @return
     */
    private boolean isSupportES2() {
        ActivityManager activityManager = (ActivityManager) getContext().getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 =
                configurationInfo.reqGlEsVersion >= 0x20000
                        || (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                        && (Build.FINGERPRINT.startsWith("generic")
                        || Build.FINGERPRINT.startsWith("unknown")
                        || Build.MODEL.contains("google_sdk")
                        || Build.MODEL.contains("Emulator")
                        || Build.MODEL.contains("Android SDK built for x86")));
        return supportsEs2;
    }


    public Earth getEarth(){
        return mEarth;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event){
       return mEarth.handleTouchEvent(event);
    }

    @Override
    public boolean onGenericMotion(View v, MotionEvent event){
        return false;
    }
}
