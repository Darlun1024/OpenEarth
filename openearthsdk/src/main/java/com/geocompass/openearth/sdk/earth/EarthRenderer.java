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
        glSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY); //需要时再绘制，还有另外一种模式是持续绘制 16ms重新绘制一次
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

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }


    /**
     * 自由旋转球体
     * @param point1
     * @param point2
     */
    protected void rotateEarth(float[] point1,float[] point2){
        nativeRotateEarth(point1,point2);
        glSurfaceView.requestRender();
    }


    /**
     * 设置放大比例[1.0,2.0)
     * 用于处理在两个zoom之间的地球放大问题
     * @param scale
     */
    protected void setScale(float scale){
        nativeSetScale(scale);
        glSurfaceView.requestRender();
    }

    /**
     * 设置显示级别,级别越大，地球半径越大
     * @param zoom
     */
    protected void setZoom(float zoom){
        nativeSetZoom(zoom);
        glSurfaceView.requestRender();
    }

    protected int getZoom(){
        return nativeGetZoom();
    }

    protected float getScale(){
        return nativeGetScale();
    }

    /**
     * 设置视角的角度(向上看或者向下看)
     * @param tilt
     */
    protected void setTilt(float tilt){
        nativeSetTilt(tilt);
        glSurfaceView.requestRender();
    }

    /**
     * 屏幕坐标转世界坐标
     * @param point
     * @return
     */
    protected float[] screen2World(float[] point){
        float[] worldPoint =  nativeScreen2World(point);
        return worldPoint;
    }

    /**
     * 世界坐标转屏幕坐标
     * @param point
     * @return
     */
    protected float[] world2Screen(float[] point){
        float[] screenPoint =  nativeWorld2Screen(point);
        return screenPoint;
    }

    /**
     * 屏幕坐标转经纬度
     * @param point
     * @return
     */
    protected float[] screen2LatLng(float[] point){
        float[] worldPoint =  nativeScreen2LatLng(point);
        return worldPoint;
    }

    /**
     * 经纬度转屏幕坐标
     * @param point
     * @return
     */
    protected float[] latLng2Screen(float[] point){
        float[] screenPoint =  nativeLatLng2Screen(point);
        return screenPoint;
    }


    /**
     * 设置屏幕中心的经纬度
     * @param latlng
     */
    protected void setCenter(float[] latlng){
        nativeSetCenter(latlng);
        glSurfaceView.requestRender();
    }


    public AssetManager getAssetManager(){
        AssetManager manager = glSurfaceView.getContext().getAssets();
        return manager;
    }

    private native void  nativeSurfaceCreated();
    private native void  nativeSurfaceChanged(int width,int height);
    private native void  nativeRender();
    private native void  nativeRotateEarth(float[] screenPoint1,float[] screenPoint2);
    private native void  nativeSetScale(float scale);
    private native float   nativeGetScale();
    private native void  nativeSetTilt(float radian);
    private native void  nativeSetZoom(float zoom);
    private native int   nativeGetZoom();
    private native void  nativeSetCenter(float[] latlng);
    private native void  nativeInitialize();
    private native float[] nativeScreen2World(float[] point);
    private native float[] nativeWorld2Screen(float[] point);
    private native float[] nativeLatLng2Screen(float[] point);
    private native float[] nativeScreen2LatLng(float[] point);

}
