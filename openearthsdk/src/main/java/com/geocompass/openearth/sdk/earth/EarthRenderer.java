package com.geocompass.openearth.sdk.earth;

import android.content.res.AssetManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY;

/**
 * Created by gxsn on 2017/11/13.
 */

public class EarthRenderer implements GLSurfaceView.Renderer  {

    private GLSurfaceView glSurfaceView;
    public EarthRenderer(GLSurfaceView glSurfaceView){
        this.glSurfaceView = glSurfaceView;
       initialize();
    }

    private void initialize(){
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        glSurfaceView.setEGLConfigChooser(new EGLConfigChooser());
        glSurfaceView.setRenderer(this);
        glSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY);
    }


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

    public AssetManager getAssetManager(){

        AssetManager manager = glSurfaceView.getContext().getAssets();
        return manager;
    }

    private native void  nativeSurfaceCreated();
    private native void  nativeSurfaceChanged(int width,int height);
    private native void  nativeRender();

}
