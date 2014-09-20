package com.homeauto.homeautoandroid.model;

public class Module {
    private String id;
    private String name;
    private String address;
    private String clientSocket;
    private int type;

    public Module(String id, String name, int type, String address, String clientSocket) {
        this.id = id;
        this.type = type;
        this.name = name;
        this.address = address;
        this.clientSocket = clientSocket == null ? "none" : clientSocket;
    }

    public String getClientSocket() {
        return clientSocket;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public int getType() {
        return type;
    }

    public String getAddress() {
        return address;
    }

    public boolean equals(Object m) {
        return (m instanceof Module) && ((Module)m).getId().equals(this.id);
    }

    public String toString() {
        return "{id : " + id + ", type = " + type +"}";
    }
}
