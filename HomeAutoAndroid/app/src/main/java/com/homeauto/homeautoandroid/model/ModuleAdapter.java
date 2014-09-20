package com.homeauto.homeautoandroid.model;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Switch;
import android.widget.TextView;

import com.homeauto.homeautoandroid.App.App;
import com.homeauto.homeautoandroid.R;

import org.w3c.dom.Text;

import java.util.List;

public class ModuleAdapter extends ArrayAdapter<Module> {
    private final Context context;
    private List<Module> modules;

    public ModuleAdapter(Context context, int resource, List<Module> modules) {
        super(context, resource, modules);
        this.context = context;
        this.modules = modules;
    }

    public View getView(int position, View convertView, ViewGroup parent) {
        Module module = modules.get(position);

        LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View rowView;

        Log.v("LOG",module.toString());

        switch (module.getType()) {
            case App.ModuleType.ON_OFF:
                rowView = inflater.inflate(R.layout.list_item_on_off, parent, false);
                break;
            case App.ModuleType.PUSH:
                rowView = inflater.inflate(R.layout.list_item_poke, parent, false);
                break;
            default:
                // bail out if the type is not recognized
                return null;
        }

        // Append description
        TextView text = (TextView) rowView.findViewById(R.id.description_text);
        text.setText("Address : " + module.getAddress() + "\nSocket : " + module.getClientSocket());

        // Append name
        TextView nameText = (TextView) rowView.findViewById(R.id.name_text);
        nameText.setText(module.getName());

        // Initialize the ToggleButton TODO: we might need to do the interface trick and put it in Modules
        View button = rowView.findViewById(R.id.toggle_switch);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.v("LOG", "clicked");
            }
        });

        return rowView;
    }
}
