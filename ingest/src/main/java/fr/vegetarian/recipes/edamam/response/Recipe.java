package fr.vegetarian.recipes.edamam.response;

import java.util.List;
import java.util.Map;

public class Recipe {

    public String uri;
    public String label;
    public String image;
    public String source;
    public String sourceIcon;
    public String url;
    public String shareAs;

    public Integer yield;

    public List<String> dietLabels;
    public List<String> healthLabels;
    public List<String> cautions;

    public List<String> ingredientLines;
    public List<Ingredient> ingredients;

    public Float calories;
    public Float totalWeight;

    public Map<String, NutrientInfo> totalNutrients;
    public Map<String, NutrientInfo> totalDaily;
    public List<NutrientDigest> digest;

    public Recipe() {

    }
}