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
    private static final String TAG = "HttpRequest";
    protected Request mRequest;
    protected Call mCall;
    protected  static OkHttpClient mClient;
    protected long mNativePtr;
    static {
        mClient = new OkHttpClient();
    }
    public HttpRequest(long nativePtr,String url){
        mNativePtr = nativePtr;
        Request.Builder builder = new Request.Builder();
        builder.url(url).method("GET",null);
        mRequest = builder.build();
        mCall = mClient.newCall(mRequest);
        mCall.enqueue(this);
    }

    public void request(long nativePtr,String url){

    }

    @Override
    public void onFailure(Call call, IOException e) {
        Log.e("tag","failure");
        nativeOnFailure(call.request().url().toString(),e.getMessage());
    }

    @Override
    public void onResponse(Call call, Response response) throws IOException {
        String url = response.request().url().toString();
        byte[] bytes = response.body().bytes();
        nativeOnResponse(url,bytes);
    }

    public void cancel(){
        mCall.cancel();
    }


    private native void nativeOnResponse(String url, byte[] body);
    private native void nativeOnFailure(String url,String message);
}
