package com.example.alex.homeauto.model;

import com.google.gson.annotations.SerializedName;

/**
 * Created by Alex on 1/20/2015.
 */
public enum ModuleType {
    @SerializedName("poke")
    POKE,

    @SerializedName("flip")
    FlIP,

    @SerializedName("rotary")
    ROTARY
}
