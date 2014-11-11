package com.homeauto.homeautoandroid.Application;

import java.util.UUID;

public class App {
    /* Device UUID */
    public static UUID DEVICE_UUID = UUID.randomUUID();

    /* making this immutable */
    private App(){};

    /**
     * URL needed for http requests
     */
    public final static class Url {
        private Url(){}

        // private final static String BASE_URL = "http://143.215.206.135:5000/"; // RSP
        // private final static String BASE_URL = "http://143.215.102.73:5000/"; // Alex computer

        // Apriary Mocked Server
        private final static String BASE_URL = "http://private-e8ccb-homeauto1.apiary-mock.com/";

        public final static String GET_ALL_MODULES = BASE_URL + "modules";
    }

    /**
     * Enumeration of different module types
     */
    public final static class ModuleType {
        private ModuleType(){}
        public final static int ON_OFF = 0;
        public final static int ROTATION = 1;
        public final static int PUSH = 2;
    }
}
