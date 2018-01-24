package com.geocompass.openearth.sdk.http;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
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
    static CallbackThread mCallbackThread;
    static {
        mClient = new OkHttpClient();
        mCallbackThread = new CallbackThread("call_back_thread");
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
//        nativeOnFailure(call.request().url().toString(),e.getMessage());
    }

    @Override
    public void onResponse(Call call, final Response response) throws IOException {
       mCallbackThread.post(new Runnable() {
           @Override
           public void run() {
               String url = response.request().url().toString();
               byte[] bytes = new byte[0];
               try {
                   bytes = response.body().bytes();
               } catch (IOException e) {
                   e.printStackTrace();
               }
               nativeOnResponse(url,bytes);
           }
       });
//        String url = response.request().url().toString();
//        byte[] bytes = response.body().bytes();
//        nativeOnResponse(url,bytes);
    }

    public void cancel(){
        mCall.cancel();
    }

    static class  CallbackThread extends  Thread{
        private Handler mHandler;
        public  CallbackThread(String name){
            super(name);
            this.setName(name);
            start();
        }

        @Override
        public void run() {
            Looper.prepare();
            mHandler = new Handler()
            {
                public void handleMessage(android.os.Message msg)
                {
                    Log.e("TAG",Thread.currentThread().getName());
                }
            };

            Looper.loop();
        }

        public void dispatchMessage(Message message){
            if(mHandler!=null)
                mHandler.dispatchMessage(message);
        }

        public void post(Runnable runnable){
            mHandler.post(runnable);
        }

        public Handler getHandler(){
            return mHandler;
        }
    }

    private native void nativeOnResponse(String url, byte[] body);
    private native void nativeOnFailure(String url,String message);
}
