package com.example.alex.homeauto.adapters;

import android.app.Activity;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.example.alex.homeauto.HomeAutoApplication;
import com.example.alex.homeauto.R;
import com.example.alex.homeauto.model.Module;
import com.example.alex.homeauto.model.ModuleType;
import com.example.alex.homeauto.net.API;

import java.util.List;

import retrofit.Callback;
import retrofit.RetrofitError;
import retrofit.client.Response;

/**
 * Created by Alex on 1/20/2015.
 */
public class ModuleAdapter extends ArrayAdapter<Module> {

    private final Context context;

    private List<Module> modules;

    public ModuleAdapter(Context context, List<Module> objects) {
        super(context, R.layout.list_item_flip, objects);
        this.context = context;
        this.modules = objects;
    }

    public View getView(int position, View convertView, ViewGroup parent)
    {
        Module module = modules.get(position);

        LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View rowView;

        switch(module.getType())
        {
            case FlIP:
                rowView = inflater.inflate(R.layout.list_item_flip, parent, false);
                TextView nameView = (TextView)rowView.findViewById(R.id.nameView);
                Switch actSwitch = (Switch)rowView.findViewById(R.id.switch1);
                nameView.setText(module.getName());
                actSwitch.setChecked(module.getValue().equals("on"));
                actSwitch.setOnCheckedChangeListener(new SwitchListener(module));
                break;
            case POKE:
                rowView = inflater.inflate(R.layout.list_item_poke, parent, false);
                break;
            case ROTARY:
                rowView = inflater.inflate(R.layout.list_item_rotary, parent, false);
                break;

            default:
                rowView = null;

        }
        return rowView;
    }

    private class SwitchListener implements CompoundButton.OnCheckedChangeListener
    {

        private Module module;

        SwitchListener(Module module)
        {
            this.module = module;
        }

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
        {
            API api = ((HomeAutoApplication)((Activity)context).getApplication()).getApi();
            module.setValue(isChecked?"on":"off");
            api.actModule(module, isChecked ? "on" : "off", new Callback<Module>() {
                @Override
                public void success(Module module, Response response) {

                }

                @Override
                public void failure(RetrofitError error) {

                }
            });
        }
    }


}
