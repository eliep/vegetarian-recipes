package fr.vegetarian.recipes.edamam.response;

import java.util.List;

public class SearchResponse {

    public int from;
    public int to;
    public int count;
    public boolean more;
    public List<Hit> hits;

    public SearchResponse() {

    }

    public boolean hasNext() {
        return (to < count);
    }
}
