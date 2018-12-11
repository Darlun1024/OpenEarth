package com.geocompass.openearth.sdk.earth.geometry;

import java.util.Locale;

/**
 * Created by gxsn on 2017/12/21.
 */

public class LatLng {

    public float lat;
    public float lon;

    public LatLng(float lat,float lon){
        this.lat = lat;
        this.lon = lon;
    }

    public String toString(){
        return  String.format(Locale.CHINA,"{%f,%f}",lat,lon);
    }
}
