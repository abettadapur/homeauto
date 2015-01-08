package com.homeauto.homeautoandroid.model;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.homeauto.homeautoandroid.Application.App;
import com.homeauto.homeautoandroid.Application.QueueHandler;

import org.json.JSONObject;

public class OnOffModule extends Module {
    public OnOffModule(String id, String name, String address, String clientSocket, Context ctx) {
        super(id, name, address, clientSocket, ctx);
        type = 0;
    }

    // TODO move this out to super class?
    // TODO we might need the interfacing trick to interact with UI
    public OnClickListener getOnClickListener() {
        return new OnClickListener() {
            @Override
            public void onClick(View view) {
                JsonObjectRequest request = new JsonObjectRequest(Request.Method.POST,
                        App.Url.MODULE + getId(),
                        null,
                        new Response.Listener<JSONObject>() {
                            @Override
                            public void onResponse(JSONObject response) {
                                Log.v("LOG", response.toString());
                            }
                        }, new Response.ErrorListener() {

                            @Override
                            public void onErrorResponse(VolleyError error) {
                                // TODO Auto-generated method stub

                            }
                        });

                // Add the request to the RequestQueue.
                QueueHandler.getInstance(mActivityContext).addToRequestQueue(request);
            }
        };
    }
}
