package com.geocompass.openearth.sdk.earth;

/**
 * Created by gxsn on 2017/12/2.
 */

public class Earth {

    //x轴方向
    public static final int ROTATEX = 0;

    //y轴方向
    public static final int ROTATEY = 1;

    //z轴方向
    public static final int ROTATEZ = 2;

    //地球半径
    public static final double EARTH_R = 6378245.0;


    private EarthRenderer mEarthRenderer;

    protected Earth(EarthRenderer renderer){
        this.mEarthRenderer = renderer;
    }


    public void rotateEarth(int axis,float degree){
        this.mEarthRenderer.rotateEarth(axis,degree);
    }

}
