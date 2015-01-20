package com.example.alex.homeauto.net;

import com.example.alex.homeauto.model.Module;

import java.util.List;

import retrofit.Callback;
import retrofit.http.Body;
import retrofit.http.GET;
import retrofit.http.POST;
import retrofit.http.Path;

/**
 * Created by Alex on 1/20/2015.
 */
public interface ModuleService
{
    @GET("/module/{id}")
    void getModule(@Path("id") String id, Callback<Module> cb);

    @POST("/module/{id}")
    void act(@Path("id") String id, @Body String value);

    @GET("/modules")
    void list(Callback<List<Module>> cb);

}
