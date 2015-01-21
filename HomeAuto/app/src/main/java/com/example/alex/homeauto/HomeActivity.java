package com.example.alex.homeauto;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import com.example.alex.homeauto.adapters.ModuleAdapter;
import com.example.alex.homeauto.model.Module;
import com.example.alex.homeauto.model.ModuleType;
import com.example.alex.homeauto.net.API;

import java.util.ArrayList;
import java.util.List;

import retrofit.Callback;
import retrofit.RetrofitError;
import retrofit.client.Response;


public class HomeActivity extends ActionBarActivity {


    private API mApi;
    List<Module> mModuleList;
    private ListView mModuleView;
    private ModuleAdapter mModuleAdapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        mModuleList = new ArrayList<>();
        mModuleView = (ListView)findViewById(R.id.moduleView);
        mModuleAdapter  = new ModuleAdapter(this, mModuleList);
        mModuleView.setAdapter(mModuleAdapter);

        mApi = ((HomeAutoApplication)this.getApplication()).getApi();

        mApi.listModules(new ListCallback());

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_home, menu);
        return true;

    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private class ListCallback implements Callback<List<Module>>
    {

        @Override
        public void success(List<Module> modules, Response response) {
            mModuleList = modules;
            mModuleAdapter.clear();
            mModuleAdapter.addAll(mModuleList);
            mModuleAdapter.notifyDataSetChanged();
        }

        @Override
        public void failure(RetrofitError error) {

            Log.e(error.getMessage(), "RetroError");

            mModuleList = new ArrayList<Module>();
            Module module = new Module();
            module.setId("IDDDDD");
            module.setType(ModuleType.FlIP);
            mModuleList.add(module);

            module = new Module();
            module.setId("IDDDDD");
            module.setType(ModuleType.FlIP);
            mModuleList.add(module);

            mModuleAdapter.clear();
            mModuleAdapter.addAll(mModuleList);
            mModuleAdapter.notifyDataSetChanged();
        }
    }
}
