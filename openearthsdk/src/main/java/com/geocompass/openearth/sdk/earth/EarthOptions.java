package com.geocompass.openearth.sdk.earth;

import com.geocompass.openearth.sdk.earth.geometry.LatLng;

/**
 * Created by gxsn on 2018/1/26.
 */

public class EarthOptions {
    public LatLng center = new LatLng(0,0);
    public float  scale = 1.0f;
    public int    zoom  = 4;
    public float  tilt  = 0;
    public int minZoom = 1;
    public int maxZoom = 16;

    public EarthOptions(){

    }

    public EarthOptions scale(float scale){
        this.scale = scale;
        return this;
    }

    public EarthOptions center(LatLng latLng){
        this.center = latLng;
        return this;
    }

    public EarthOptions zoom(int zoom){
        this.zoom = zoom;
        return this;
    }

    public EarthOptions tilt(float tile){
        this.tilt = tilt;
        return this;
    }

    public EarthOptions minZoom(int minZoom){
        this.minZoom = minZoom;
        return this;
    }

    public EarthOptions maxZoom(int maxZoom){
        this.maxZoom = maxZoom;
        return this;
    }


}
