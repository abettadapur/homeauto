package com.homeauto.homeautoandroid.model;

public class Module {
    private String id;
    private int type;

    public Module(String id, int type) {
        this.id = id;
        this.type = type;
    }

    public String getId() {
        return id;
    }

    public boolean equals(Object m) {
        return (m instanceof Module) && ((Module)m).getId().equals(this.id);
    }
}
