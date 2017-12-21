package com.geocompass.openearth.sdk.earth;

import android.content.Context;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

import com.almeros.android.multitouch.MoveGestureDetector;
import com.almeros.android.multitouch.RotateGestureDetector;
import com.almeros.android.multitouch.ShoveGestureDetector;

/**
 * Created by gxsn on 2017/12/8.
 */

public class GestureDetector  implements MoveGestureDetector.OnMoveGestureListener,
        RotateGestureDetector.OnRotateGestureListener,
        ShoveGestureDetector.OnShoveGestureListener,ScaleGestureDetector.OnScaleGestureListener {
    private MoveGestureDetector   mMoveGestureDetector;
    private RotateGestureDetector mRotateGestureDetector;
    private ShoveGestureDetector  mShoveGestureDetector;
    private ScaleGestureDetector  mScaleGestureDetector;
    private Earth mEarth;
    public GestureDetector(Context context,Earth earth){
        mMoveGestureDetector   = new MoveGestureDetector(context,this);
        mRotateGestureDetector = new RotateGestureDetector(context,this);
        mShoveGestureDetector  = new ShoveGestureDetector(context,this);
        mScaleGestureDetector  = new ScaleGestureDetector(context,this);
    }
    public boolean onTouchEvent(MotionEvent event){
        mMoveGestureDetector.onTouchEvent(event);
        mRotateGestureDetector.onTouchEvent(event);
        mShoveGestureDetector.onTouchEvent(event);
        mScaleGestureDetector.onTouchEvent(event);
        return false;
    }


    //shove
    @Override
    public boolean onShove(ShoveGestureDetector detector) {
        return false;
    }

    @Override
    public boolean onShoveBegin(ShoveGestureDetector detector) {

        return false;
    }

    @Override
    public void onShoveEnd(ShoveGestureDetector detector) {

    }

    //rotate
    @Override
    public boolean onRotate(RotateGestureDetector detector) {
        return false;
    }

    @Override
    public boolean onRotateBegin(RotateGestureDetector detector) {
        return false;
    }

    @Override
    public void onRotateEnd(RotateGestureDetector detector) {

    }

    //move
    @Override
    public boolean onMove(MoveGestureDetector detector) {

        return false;
    }

    @Override
    public boolean onMoveBegin(MoveGestureDetector detector) {

        return false;
    }

    @Override
    public void onMoveEnd(MoveGestureDetector detector) {

    }

    //scale
    float mStartSpan; //开始的距离
    @Override
    public boolean onScale(ScaleGestureDetector detector) {
        float span = detector.getCurrentSpan();
        float sacle = span/mStartSpan;
        return false;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector) {
        mStartSpan = detector.getCurrentSpan();
        return false;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector) {

    }
}
