package com.example.alex.homeauto;

import android.app.Application;

import com.example.alex.homeauto.net.API;

/**
 * Created by Alex on 1/20/2015.
 */
public class HomeAutoApplication extends Application
{
    private API mApi;

    public void onCreate()
    {
        super.onCreate();
        mApi = new API("http://private-9b9fc-homeauto1.apiary-mock.com");
    }

    public API getApi()
    {
        return mApi;
    }


}

