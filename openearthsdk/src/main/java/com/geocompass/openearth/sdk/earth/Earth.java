package com.geocompass.openearth.sdk.earth;

/**
 * Created by gxsn on 2017/12/2.
 */

public class Earth {
    private static final float MAX_ZOOM = 18.0f;
    private static final float MIN_ZOOM = 1.0f;
    private float zoom = 1.0f;
    private float tilt = 0.0f;
    private EarthRenderer mEarthRenderer;

    protected Earth(EarthRenderer renderer){
        this.mEarthRenderer = renderer;
    }


    public void rotateEarth(int axis,float degree){
        this.mEarthRenderer.rotateEarth(axis,degree);
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
}
