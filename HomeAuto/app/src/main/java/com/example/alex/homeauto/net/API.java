package com.example.alex.homeauto.net;

import com.example.alex.homeauto.model.Module;

import java.util.Dictionary;
import java.util.Hashtable;
import java.util.List;

import retrofit.Callback;
import retrofit.RestAdapter;

/**
 * Created by Alex on 1/20/2015.
 */
public class API
{
    private String endpoint;
    private ModuleService moduleService;

    public API(String endpoint)
    {
        this.endpoint = endpoint;
        RestAdapter restAdapter = new RestAdapter.Builder()
                .setEndpoint(endpoint)
                .build();

        moduleService = restAdapter.create(ModuleService.class);
    }

    public void listModules(Callback<List<Module>> callback)
    {
        moduleService.list(callback);
    }

    public void actModule(Module module, String value, Callback<Module> cb)
    {
        Dictionary<String, String> jsonBody = new Hashtable<>();
        jsonBody.put("action", value);
        moduleService.act(module.getId(), jsonBody, cb);
    }

    public void getModule(String id, Callback<Module> cb)
    {
        moduleService.getModule(id, cb);
    }

}
