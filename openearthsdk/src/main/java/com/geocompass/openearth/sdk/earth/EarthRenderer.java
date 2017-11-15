package com.geocompass.openearth.sdk.earth;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by gxsn on 2017/11/13.
 */

public class EarthRenderer implements GLSurfaceView.Renderer  {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0,0,width,height);
        nativeSurfaceChanged(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    private native void  nativeSurfaceCreated();
    private native void  nativeSurfaceChanged(int width,int height);
    private native void  nativeRender();

}
