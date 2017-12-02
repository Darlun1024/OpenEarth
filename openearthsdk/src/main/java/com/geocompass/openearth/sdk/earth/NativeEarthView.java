package com.geocompass.openearth.sdk.earth;

import android.util.Log;

/**
 * Created by gxsn on 2017/11/14.
 */

public final class NativeEarthView {


    private static  final  String TAG = "NativeEarthView";
    private EarthView mEarthView;
    private EarthRenderer mEarthRenderer;

    public NativeEarthView(EarthView earthView,EarthRenderer renderer){
        mEarthView    = earthView;
        mEarthRenderer = renderer;
        Log.e(TAG,getStringFromJNI());

    }

    private void  initializeEarth(){

    }

    public native String getStringFromJNI();

    public native void nativeInitializeEarth(EarthRenderer renderer);

}
