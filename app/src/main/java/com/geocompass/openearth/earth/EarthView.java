package com.geocompass.openearth.earth;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.FrameLayout;

/**
 * Created by gxsn on 2017/11/13.
 */

public class EarthView extends FrameLayout {

    EarthRenderer earthRenderer = new EarthRenderer();

    public EarthView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }



}
