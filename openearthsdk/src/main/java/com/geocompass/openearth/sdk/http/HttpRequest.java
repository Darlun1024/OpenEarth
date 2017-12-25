package com.geocompass.openearth.sdk.http;

import android.util.Log;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

/**
 * copy from mapbox
 * Created by gxsn on 2017/12/25.
 */

public class HttpRequest implements Callback{
    protected Request mRequest;
    protected Call mCall;
    private OkHttpClient mClient;
    public HttpRequest(String url){
        Request.Builder builder = new Request.Builder();
        builder.url(url).method("GET",null);
        mClient = new OkHttpClient();
        mRequest = builder.build();
        mCall = mClient.newCall(mRequest);
        mCall.enqueue(this);
    }
    @Override
    public void onFailure(Call call, IOException e) {
        Log.e("tag","failure");
        nativeOnFailure(e.getMessage());
    }

    @Override
    public void onResponse(Call call, Response response) throws IOException {
        Log.e("tag","respone");
        nativeOnResponse(response.code(),response.body().bytes());
    }


    private native void nativeOnResponse(int code, byte[] body);
    private native void nativeOnFailure(String message);
}
