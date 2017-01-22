package fr.vegetarian.recipes.config;

import java.util.ArrayList;
import java.util.List;

public class EdamamApiConfig {

    public String appId;
    public String appKey;
    public List<String> query = new ArrayList<String>();
    public String diet = null;
    public String health = "vegetarian";
    public int querySize = 100;

    public EdamamApiConfig() {

    }
}
