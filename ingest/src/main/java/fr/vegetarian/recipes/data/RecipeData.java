package fr.vegetarian.recipes.data;

import com.fasterxml.jackson.databind.ObjectMapper;
import fr.vegetarian.recipes.edamam.response.Ingredient;
import fr.vegetarian.recipes.edamam.response.NutrientDigest;
import fr.vegetarian.recipes.edamam.response.Recipe;
import org.apache.commons.codec.digest.DigestUtils;

import java.io.IOException;
import java.util.*;

public class RecipeData {

    public enum RecipeFeatureType {
        NUTRIENT_DAILY {
            @Override
            public Map<String, Double> extract(Recipe recipe) {
                Map<String, Double> features = new HashMap(30);
                for (NutrientDigest digest: recipe.digest) {
                    features.put(digest.label, digest.daily);
                }
                return features;
            }
        },
        NUTRIENT_TOTAL{
            @Override
            public Map<String, Double> extract(Recipe recipe) {
                Map<String, Double> features = new HashMap(30);
                for (NutrientDigest digest: recipe.digest) {
                    features.put(digest.label, digest.total);
                }
                return features;
            }
        },
        INGREDIENT_TOTAL{
            @Override
            public Map<String, Double> extract(Recipe recipe) {
                Map<String, Double> features = new HashMap(30);
                for (Ingredient ingredient: recipe.ingredients) {
                    double weight = ingredient.weight;
                    if (features.containsKey(ingredient.food))
                        weight+= features.get(ingredient.food);
                    features.put(ingredient.food, weight );
                }
                return features;
            }
        };

        abstract public Map<String, Double> extract(Recipe recipe);
    }

    protected String id;
    protected String title;

    protected String json;
    protected Map<String, Double> features;

    private RecipeData(String id, String title, Map<String, Double> features, String json) {
        this.id = DigestUtils.md5Hex(DigestUtils.md5Hex(id));
        this.title = title;
        this.features = features;
        this.json = json;
    }

    public String getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public Map<String, Double> getFeatures() {
        return features;
    }

    public String csv(Set<String> featureNames) {
        String line = id;
        for (String featureName: featureNames) {
            String featureValue = ((features.containsKey(featureName)) ? features.get(featureName).toString() : "");
            line += ";" +  featureValue;
        }

        return line;
    }

    public String toString() {
        return json;
    }

    public static Optional<RecipeData> buildFromRecipe(Recipe recipe, RecipeFeatureType featureType, ObjectMapper mapper) {
        if (recipe == null || recipe.url == null || recipe.label == null || recipe.digest == null || recipe.digest.size() == 0)
            return Optional.empty();

        RecipeData recipeData = null;
        Map<String, Double> features = featureType.extract(recipe);
        try {
            String json = mapper.writeValueAsString(recipe);
            recipeData = new RecipeData(recipe.url, recipe.label, features, json);
        } catch (IOException e) {}

        return Optional.of(recipeData);
    }
}
