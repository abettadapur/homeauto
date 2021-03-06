package com.homeauto.homeautoandroid.model;

import static com.homeauto.homeautoandroid.Application.App.ModuleType;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.homeauto.homeautoandroid.R;

import java.util.List;

/**
 * ModuleAdapter
 *
 * Adapter class for modules
 */
public class ModuleAdapter extends ArrayAdapter<Module> {
    /* context ref */
    private final Context context;

    /* module list */
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

        Log.v("LOG","called getView");

        // TODO: subclass?
        switch (module.getType()) {
            case ModuleType.ON_OFF:
                rowView = inflater.inflate(R.layout.list_item_on_off, parent, false);
                break;
            case ModuleType.PUSH:
                rowView = inflater.inflate(R.layout.list_item_poke, parent, false);
                break;
            case ModuleType.ROTATION:
                rowView = inflater.inflate(R.layout.list_item_spin, parent, false);
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

        // Initialize the ToggleButton
        // TODO: we might need to do the interface trick and put it in Modules
        View button = rowView.findViewById(R.id.toggle_switch);
        button.setOnClickListener(module.getOnClickListener());

        return rowView;
    }
}
