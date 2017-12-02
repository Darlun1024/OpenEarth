package com.geocompass.openearth;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.geocompass.openearth.sdk.earth.Earth;
import com.geocompass.openearth.sdk.earth.EarthView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private EarthView mEarthView;
    private Earth mEarth;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mEarthView = findViewById(R.id.earth_view);

        mEarth = mEarthView.getEarth();

        findViewById(R.id.btn_rotate).setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_rotate:
                //绕Y轴，旋转10度
            mEarth.rotateEarth(1, (float) (Math.PI/72));
            return;
        }
    }
}
