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
    static {
        System.loadLibrary("lib-earth");
    }

    private EarthRenderer mEarthRenderer;
    private GLSurfaceView mSurfaceView;
    private NativeEarthView mNativeEarthView;
    private Earth mEarth;

    private MotionEvent.PointerCoords mPrePointer;

    public EarthView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize();
    }


    private void initialize() {
        //调整其它View的位置

        //初始化SurfaceView
        View view = LayoutInflater.from(getContext()).inflate(R.layout.earth_view, this);
        mSurfaceView = view.findViewById(R.id.surface_view);
        mEarthRenderer = new EarthRenderer(mSurfaceView);
        mNativeEarthView = new NativeEarthView(this, mEarthRenderer);

        mEarth = new Earth(mEarthRenderer);
    }

    private void initSurfaceView() {

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
        int action = event.getAction();
        switch (action){
            case MotionEvent.ACTION_DOWN:
                float x = event.getX();
                float y = event.getY();
                float[] array = {x,y};
                float[] world =  mEarthRenderer.screen2World(array);
                float[] srceen1 = mEarthRenderer.world2Screen(world);
                float[] latlng = mEarthRenderer.screen2LatLng(array);
                float[] srceen = mEarthRenderer.latLng2Screen(latlng);
                break;

            case MotionEvent.ACTION_MOVE:break;

            case MotionEvent.ACTION_UP:break;
        }
        return false;
    }

    @Override
    public boolean onGenericMotion(View v, MotionEvent event){
        return false;
    }
}
