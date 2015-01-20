package com.example.alex.homeauto.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;

import com.example.alex.homeauto.R;
import com.example.alex.homeauto.model.Module;
import com.example.alex.homeauto.model.ModuleType;

import java.util.List;

/**
 * Created by Alex on 1/20/2015.
 */
public class ModuleAdapter extends ArrayAdapter<Module> {

    private final Context context;

    private List<Module> modules;

    public ModuleAdapter(Context context, int resource, List<Module> objects) {
        super(context, resource, objects);
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
}
