package fr.vegetarian.recipes.data;

import org.apache.commons.lang3.StringUtils;

import java.util.*;

public class RecipeDataset {

    protected Set<String> featureNames;

    protected Map<String, RecipeData> recipes;

    public RecipeDataset() {
        featureNames = new LinkedHashSet<>(30);
        recipes = new HashMap<>(5000);
    }

    public Set<String> getFeatureNames() {
        return featureNames;
    }

    public Map<String, RecipeData> getRecipes() {
        return recipes;
    }

    public boolean contains(RecipeData recipe) {
        return recipes.containsKey(recipe.getId());
    }

    public void add(RecipeData recipe) {
        for (String feature: recipe.getFeatures().keySet()) {
            featureNames.add(feature);
        }
        recipes.put(recipe.getId(), recipe);
    }

    public List<String> csv() {
        List<String> lines = new ArrayList<>(recipes.size());
        lines.add(headers());
        for (RecipeData recipe: recipes.values())
            lines.add(recipe.csv(featureNames));

        return lines;
    }

    protected String headers() {
        return "ID;" + StringUtils.join(featureNames, ";");
    }
}
