package com.homeauto.homeautoandroid.model;

import android.content.Context;
import android.util.Log;
import android.view.View;

public class Module {
    protected String id;
    protected String name;
    protected String address;
    protected String clientSocket;
    protected int type;
    protected Context mActivityContext;

    public Module(String id, String name, int type, String address, String clientSocket,
                  Context ctx) {
        this(id, name, address, clientSocket, ctx);
        this.type = type;
    }

    public Module(String id, String name, String address, String clientSocket, Context ctx){
        this.id = id;
        this.name = name;
        this.address = address;
        this.clientSocket = clientSocket == null ? "none" : clientSocket;
        this.mActivityContext = ctx;
    }

    public String getClientSocket() {
        return clientSocket;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public int getType() {
        return type;
    }

    public String getAddress() {
        return address;
    }

    public boolean equals(Object m) {
        return (m instanceof Module) && ((Module)m).getId().equals(this.id);
    }

    public int hashCode() {
        return getId().hashCode();
    }

    public View.OnClickListener getOnClickListener() {
        return new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.v("LOG", "clicked");
            }
        };
    }

    public String toString() {
        return "{id : " + id + ", type = " + type +"}";
    }
}
