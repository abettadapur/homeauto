package com.homeauto.homeautoandroid.Application;

import android.content.Context;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

/**
 * Singleton class to manage Volley's request queue
 */
public class QueueHandler {
    private static QueueHandler mInstance;
    private RequestQueue mRequestQueue;
    private static Context mContex;

    private QueueHandler(Context ctx) {
        this.mContex = ctx;
    }

    public static synchronized QueueHandler getInstance(Context context) {
        if (mInstance == null) {
            mInstance = new QueueHandler(context);
        }
        return mInstance;
    }

    public RequestQueue getRequestQueue() {
        if (mRequestQueue == null) {
            mRequestQueue = Volley.newRequestQueue(mContex.getApplicationContext());
        }
        return mRequestQueue;
    }

    public <T> void addToRequestQueue(Request<T> req) {
        getRequestQueue().add(req);
    }
}
