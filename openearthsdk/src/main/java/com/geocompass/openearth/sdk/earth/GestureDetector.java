package com.geocompass.openearth.sdk.earth;

import android.content.Context;
import android.view.MotionEvent;

import com.almeros.android.multitouch.MoveGestureDetector;
import com.almeros.android.multitouch.RotateGestureDetector;
import com.almeros.android.multitouch.ShoveGestureDetector;

/**
 * Created by gxsn on 2017/12/8.
 */

public class GestureDetector  implements MoveGestureDetector.OnMoveGestureListener,
        RotateGestureDetector.OnRotateGestureListener,
        ShoveGestureDetector.OnShoveGestureListener {
    MoveGestureDetector mMoveGestureDetector;
    RotateGestureDetector mRotateGestureDetector;
    ShoveGestureDetector mShoveGestureDetector;
    public GestureDetector(Context context){
        mMoveGestureDetector = new MoveGestureDetector(context,this);
        mRotateGestureDetector = new RotateGestureDetector(context,this);
        mShoveGestureDetector = new ShoveGestureDetector(context,this);
    }
    public boolean onTouchEvent(MotionEvent event){
        mMoveGestureDetector.onTouchEvent(event);
        mRotateGestureDetector.onTouchEvent(event);
        mShoveGestureDetector.onTouchEvent(event);
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
        detector.
        return false;
    }

    @Override
    public boolean onMoveBegin(MoveGestureDetector detector) {

        return false;
    }

    @Override
    public void onMoveEnd(MoveGestureDetector detector) {

    }
}
