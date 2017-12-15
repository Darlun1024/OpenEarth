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


    /**
     * 设置放大比例[1.0,2.0)
     * 用于处理在两个zoom之间的地球放大问题
     * @param scale
     */
    public void setScale(float scale){
        nativeSetScale(scale);
        glSurfaceView.requestRender();
    }

    /**
     * 设置显示级别,级别越大，地球半径越大
     * @param zoom
     */
    public void setZoom(float zoom){
        nativeSetZoom(zoom);
        glSurfaceView.requestRender();
    }

    /**
     * 设置视角的角度(向上看或者向下看)
     * @param tilt
     */
    public void setTilt(float tilt){
        nativeSetTilt(tilt);
        glSurfaceView.requestRender();
    }

    public float[] screen2World(float[] point){
        float[] worldPoint =  nativeScreen2World(point);
        return worldPoint;
    }

    public float[] world2Screen(float[] point){
        float[] screenPoint =  nativeWorld2Screen(point);
        return screenPoint;
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
    private native float[] nativeScreen2World(float[] point);
    private native float[] nativeWorld2Screen(float[] point);


}
