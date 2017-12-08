package com.geocompass.openearth;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.View;

import com.geocompass.openearth.sdk.earth.Earth;
import com.geocompass.openearth.sdk.earth.EarthView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private EarthView mEarthView;
    private Earth mEarth;
    float x = 0;
    float x1;
    float y = 0;
    float y1;

    float preX = 0;
    float preY = 0;

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mEarthView = findViewById(R.id.earth_view);

        mEarth = mEarthView.getEarth();

        findViewById(R.id.btn_rotate).setOnClickListener(this);


        
//        mEarthView.setOnTouchListener(this);

    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_rotate:
                //绕Y轴，旋转10度
            mEarth.rotateEarth(1, (float) (Math.PI/72));
//                mEarth.rotateEarth(Earth.ROTATEZ, (float) (Math.PI / 72));
                return;
        }
    }



//    @Override
//    public boolean onTouch(View v, MotionEvent event) {
//        switch (event.getAction()) {
//            case MotionEvent.ACTION_DOWN:
//                x = event.getX();
//                y = event.getY();
//
//                preX = event.getX();
//                preY = event.getY();
//                break;
//            case MotionEvent.ACTION_MOVE:
//                x1 = event.getX();
//                y1 = event.getY();
//
//                if (x1 < preX) {
//                    x = preX;
//                }
//                if (y1 < preY) {
//                    y = preY;
//                }
//                float v1 = x1 - x;
//                float v2 = y1 - y;
//
////                        if (Math.abs(v1) > 10 && Math.abs(v2) > 10) {
//                if (v1 > 0 && v2 > 0) {
//                    mEarth.rotateEarth(Earth.ROTATEY, (float) (Math.PI / 45));
//                    mEarth.rotateEarth(Earth.ROTATEX, (float) (Math.PI / 45));
//                } else if (v1 > 0 && v2 < 0) {
//                    mEarth.rotateEarth(Earth.ROTATEY, (float) (Math.PI / 45));
//                    mEarth.rotateEarth(Earth.ROTATEX, -(float) (Math.PI / 45));
//                } else if (v1 < 0 && v2 < 0) {
//                    mEarth.rotateEarth(Earth.ROTATEY, -(float) (Math.PI / 45));
//                    mEarth.rotateEarth(Earth.ROTATEX, -(float) (Math.PI / 45));
//                } else if (v1 < 0 && v2 > 0) {
//                    mEarth.rotateEarth(Earth.ROTATEY, -(float) (Math.PI / 45));
//                    mEarth.rotateEarth(Earth.ROTATEX, (float) (Math.PI / 45));
//                }
////                        }
//                preX = x1;
//                preY = y1;
//                break;
//            case MotionEvent.ACTION_UP:
//                break;
//        }
//        return true;
//    }
}
