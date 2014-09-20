package com.homeauto.homeautoandroid.App;

import java.util.UUID;

public class App {
    /* Device UUID */
    public static UUID DEVICE_UUID = UUID.randomUUID();

    /**
     * URL needed for http requests
     */
    public static class Url {
        // public final static String BASE_URL = "http://143.215.206.135:5000/"; // RSP
        public final static String BASE_URL = "http://143.215.102.73:5000/"; // Alex computer

        public final static String GET_ALL_MODULES = BASE_URL + "modules";
    }

    /**
     * Enumeration of different module types
     */
    public static class ModuleType {
        public final static int ON_OFF = 0;
        public final static int ROTATION = 1;
        public final static int PUSH = 2;
    }

}
