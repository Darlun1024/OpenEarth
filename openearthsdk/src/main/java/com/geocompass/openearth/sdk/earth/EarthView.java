package com.geocompass.openearth.sdk.earth;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;

import com.geocompass.openearth.sdk.R;


/**
 * Created by gxsn on 2017/11/13.
 */

public class EarthView extends FrameLayout {
    private EarthRenderer mEarthRenderer;
    private GLSurfaceView mSurfaceView;
    private NativeEarthView  mNativeEarthView;
    public EarthView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize();
    }



    private void initialize(){
        //调整其它View的位置

        //初始化SurfaceView
        View view  = LayoutInflater.from(getContext()).inflate(R.layout.earth_view,this);
        mSurfaceView = view.findViewById(R.id.surface_view);
        mEarthRenderer = new EarthRenderer();
        mSurfaceView.setRenderer(mEarthRenderer);
        mSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        mNativeEarthView = new NativeEarthView(this,mEarthRenderer);
    }

}
