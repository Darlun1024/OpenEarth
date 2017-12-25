package com.geocompass.openearth.sdk.earth;

import android.graphics.PointF;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import com.geocompass.openearth.sdk.earth.geometry.LatLng;

/**
 * Created by gxsn on 2017/12/2.
 */

public class Earth {
    private static final float MAX_ZOOM = 18.0f;
    private static final float MIN_ZOOM = 1.0f;
    private float zoom = 1.0f;
    private float tilt = 0.0f;
    private EarthRenderer mEarthRenderer;
    private GLSurfaceView mGLSurfaceView;
    private float[] preXY;
    private GestureDetector mGestureDetector;

    protected Earth(GLSurfaceView glSurfaceView){
        this.mGLSurfaceView = glSurfaceView;
        mEarthRenderer = new EarthRenderer(mGLSurfaceView);
        mGestureDetector = new GestureDetector(glSurfaceView.getContext(),this);
    }



    public void zoomIn(){
        mEarthRenderer.zoomIn();
    }

    public void zoomOut(){
       mEarthRenderer.zoomOut();
    }

    public void setZoom(int zoom){
        assert (zoom >= MIN_ZOOM && zoom <= MAX_ZOOM );
        this.mEarthRenderer.setZoom(zoom);
    }

    public void lookUp(){
        tilt += 0.1f;
        this.mEarthRenderer.setTilt(tilt);
    }

    public void lookDown(){
        tilt -= 0.1f;
        this.mEarthRenderer.setTilt(tilt);
    }

    public void setCenter(LatLng latLng){
        this.mEarthRenderer.setCenter(new float[]{latLng.lat,latLng.lon});
    }

    public void scale(float scale){
        this.mEarthRenderer.setScale(scale);
    }

    public void tilt(float tilt){
        this.mEarthRenderer.tilt(tilt);
    }

    public void rotateEarth(float[] point1,float[] point2){
        this.mEarthRenderer.rotate(point1,point2);
    }

    public LatLng screenToLatLng(PointF screenPoint){
        float[] latlng =  mEarthRenderer.screen2LatLng(new float[]{screenPoint.x,screenPoint.y});
        return new LatLng(latlng[0],latlng[1]);
    }

    public PointF latLngToScreen(LatLng latLng){
        float[] screen = mEarthRenderer.latLng2Screen(new float[]{latLng.lat,latLng.lon});
        return new PointF(screen[0],screen[1]);
    }


    protected boolean handleTouchEvent(MotionEvent event){
        return  mGestureDetector.onTouchEvent(event);
    }
}
