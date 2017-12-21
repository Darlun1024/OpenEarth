package com.geocompass.openearth.sdk.earth;

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

    protected Earth(GLSurfaceView glSurfaceView){
        this.mGLSurfaceView = glSurfaceView;
        mEarthRenderer = new EarthRenderer(mGLSurfaceView);
    }



    public void zoomIn(){
        if(zoom+1 > MAX_ZOOM) return;
        zoom = zoom+1;
        this.mEarthRenderer.setZoom(zoom);
    }

    public void zoomOut(){
        if(zoom-1 < MIN_ZOOM)return;
        zoom = zoom-1;
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


    protected boolean handleTouchEvent(MotionEvent event){
        int action = event.getAction();
        switch (action){
            case MotionEvent.ACTION_DOWN:
                float x = event.getX();
                float y = event.getY();
                preXY = new float[]{x,y};
                break;
            case MotionEvent.ACTION_MOVE:
                float x1 = event.getX();
                float y1 = event.getY();
                float[] xy = new float[]{x1,y1};
                if(Math.abs(x1-preXY[0]) < 2 && Math.abs(y1-preXY[1]) < 2) break;
                mEarthRenderer.rotateEarth(preXY,xy);
                preXY = xy;
                break;
            case MotionEvent.ACTION_UP:break;
        }
        return true;
    }
}
