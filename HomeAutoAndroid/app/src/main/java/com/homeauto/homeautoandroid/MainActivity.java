package com.homeauto.homeautoandroid;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.homeauto.homeautoandroid.App.App;
import com.homeauto.homeautoandroid.model.Module;
import com.homeauto.homeautoandroid.model.ModuleAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class MainActivity extends Activity {
    private final Context CONTEXT = this;
    private boolean running = true;

    private ModuleAdapter adapter;
    private ArrayList<Module> modules;
    private ArrayList<Boolean> moduleStates;
    private ListView listView;

    /**
     * Thread that would hit the server every 2 seconds
     * to see if there are new modules available.
     */
    private Thread syncThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        modules = new ArrayList<Module>();
        adapter = new ModuleAdapter(CONTEXT, R.layout.list_item, modules);

        listView = ((ListView)findViewById(R.id.module_list));
        listView.setAdapter(adapter);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                view.setActivated(!view.isActivated());
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        running = true;

        syncThread = new Thread() {
            @Override
            public void run() {
                try {
                    while(true) {
                        if (!running) break;
                        scanForAvailableModules();
                        sleep(2000);
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        };

        syncThread.start();
    }

    @Override
    protected void onPause() {
        super.onPause();
        running = false;
        try {
            syncThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    /**
     * This call will scan for all available modules.
     *
     * TODO move to AutoHomeApplication Class.
     */
    private void scanForAvailableModules() {
        // Instantiate the RequestQueue.
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = App.BASE_URL + "modules";

        // Request a string response from the provided URL.
        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.v("LOG", response.toString());
                        JSONArray resModules = null;
                        try {
                            resModules = response.getJSONArray("modules");
                        } catch (JSONException e) {
                            e.printStackTrace();
                            // bail early if we didn't get a legit response.
                            return;
                        }

                        ArrayList<Module> removeList = new ArrayList<Module>();
                        for (int i=0; i<resModules.length(); i++) {
                            try {
                                JSONObject obj = resModules.getJSONObject(i);

                                Module tmp = new Module(obj.getString("id"), obj.getInt("type"));

                                if (!modules.contains(tmp)) {
                                    modules.add(tmp);
                                } else {
                                    removeList.remove(tmp);
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                                return;
                            }
                        }

                        for (Module m : removeList) {
                            modules.remove(m);
                        }

                        adapter.notifyDataSetChanged();
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e("LOG", error.getMessage());
                    }
            });
        // Add the request to the RequestQueue.
        queue.add(request);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
