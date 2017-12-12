package com.geocompass.openearth.sdk.earth;

/**
 * Created by gxsn on 2017/12/2.
 */

public class Earth {

    private EarthRenderer mEarthRenderer;

    protected Earth(EarthRenderer renderer){
        this.mEarthRenderer = renderer;
    }


    public void rotateEarth(int axis,float degree){
        this.mEarthRenderer.rotateEarth(axis,degree);
    }

}
