package com.example.alex.homeauto.model;

/**
 * Created by Alex on 1/20/2015.
 */
public class Module
{
    private String id;
    private ModuleType type;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public ModuleType getType() {
        return type;
    }

    public void setType(ModuleType type) {
        this.type = type;
    }

    public Module()
    {

    }
}
