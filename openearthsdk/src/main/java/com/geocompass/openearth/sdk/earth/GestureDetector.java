package com.geocompass.openearth.sdk.earth;

import android.content.Context;
import android.util.Log;
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
    private static final String TAG = "GestureDetector";
    private MoveGestureDetector   mMoveGestureDetector;
    private RotateGestureDetector mRotateGestureDetector;
    private ShoveGestureDetector  mShoveGestureDetector;
    private ScaleGestureDetector  mScaleGestureDetector;
    private Earth mEarth;
    private float [] preXY = {-1,-1};
    public GestureDetector(Context context,Earth earth){
        mEarth = earth;
        mMoveGestureDetector   = new MoveGestureDetector(context,this);
        mRotateGestureDetector = new RotateGestureDetector(context,this);
        mShoveGestureDetector  = new ShoveGestureDetector(context,this);
        mScaleGestureDetector  = new ScaleGestureDetector(context,this);
    }

    public boolean onTouchEvent(MotionEvent event){
//        mMoveGestureDetector.onTouchEvent(event);
        mRotateGestureDetector.onTouchEvent(event);
        mShoveGestureDetector.onTouchEvent(event);
        mScaleGestureDetector.onTouchEvent(event);
        int action = event.getAction();
        switch (action){
            case MotionEvent.ACTION_DOWN:
                preXY = new float[]{event.getX(),event.getY()};
                break;
            case MotionEvent.ACTION_MOVE:
                if(mShoveGestureDetector.isInProgress()||mRotateGestureDetector.isInProgress()||mScaleGestureDetector.isInProgress()){
                    preXY = new float[]{event.getX(),event.getY()};
                    break;
                }
                if(preXY[0] < 0 ){
                    preXY = new float[]{event.getX(),event.getY()};
                    break;
                }
                float x1 = event.getX();
                float y1 = event.getY();
                float[] xy = new float[]{x1,y1};
                if(Math.abs(x1-preXY[0]) < 2 && Math.abs(y1-preXY[1]) < 2) break;
                mEarth.rotateEarth(preXY,xy);
                preXY = xy;
                break;
            case MotionEvent.ACTION_UP:
                preXY = new float[]{-1,-1};
                break;
            case MotionEvent.ACTION_POINTER_UP:
                //不管几个手指，先把prexy设置为（-1，-1）
                preXY = new float[]{-1,-1};
                break;
            case MotionEvent.ACTION_POINTER_DOWN:
                break;
        }
        return true;
    }


    //shove
    @Override
    public boolean onShove(ShoveGestureDetector detector) {
        Log.e(TAG,"shove delta:"+detector.getShovePixelsDelta());
        float delta = detector.getShovePixelsDelta();
        float tilt = delta * 0.005f;
        mEarth.tilt(tilt);
        return true;
    }

    @Override
    public boolean onShoveBegin(ShoveGestureDetector detector) {

        return true;
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

        return true;
    }

    @Override
    public boolean onMoveBegin(MoveGestureDetector detector) {

        return true;
    }

    @Override
    public void onMoveEnd(MoveGestureDetector detector) {

    }

    //scale
    @Override
    public boolean onScale(ScaleGestureDetector detector) {
        float span = detector.getCurrentSpan();
        float preSpan = detector.getPreviousSpan();
        float scale = span/preSpan;
        if(0.9f < scale && scale < 1.1)return false;
        mEarth.scale(scale);
        Log.e(TAG,"scale:"+scale);
        return true;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector) {
        return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector) {
    }
}
