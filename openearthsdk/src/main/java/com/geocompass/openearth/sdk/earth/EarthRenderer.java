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
        nativeInitialize();
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

    /**
     *
     * @param axis 围绕旋转的坐标轴  0 X轴,1 Y轴 ，2 Z轴
     * @param radian 旋转的弧度
     */
    public void rotateEarth(int axis,float radian){
        nativeRotateEarth(axis,radian);
        glSurfaceView.requestRender();
    }


    public void setScale(float scale){
        nativeSetScale(scale);
        glSurfaceView.requestRender();
    }

    public void setZoom(float zoom){
        nativeSetZoom(zoom);
        glSurfaceView.requestRender();
    }

    public void setTilt(float tilt){
        nativeSetTilt(tilt);
        glSurfaceView.requestRender();
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
    private native void  nativeRotateEarth(int axis,float radian);
    private native void  nativeSetScale(float scale);
    private native void  nativeSetTilt(float radian);
    private native void  nativeSetZoom(float zoom);
    private native void  nativeInitialize();


}
