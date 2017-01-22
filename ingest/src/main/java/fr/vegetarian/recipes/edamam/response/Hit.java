package fr.vegetarian.recipes.edamam.response;

import java.util.List;

public class Hit {
    public Recipe recipe; // Matching recipe (Recipe object)
    public boolean bookmarked; // Is this recipe bookmarked by the searching user?
    public boolean bought; // Is this recipe bought by the searching user?

    public Hit() {

    }
}
