package com.homeauto.homeautoandroid.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.homeauto.homeautoandroid.R;

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
        LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View rowView = inflater.inflate(R.layout.list_item, parent, false);

        // Append description
        TextView text = (TextView) rowView.findViewById(R.id.description_text);
        text.setText(modules.get(position).getId());

        return rowView;
    }
}
