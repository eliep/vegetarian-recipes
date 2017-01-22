package fr.vegetarian.recipes.edamam.client;

public class SearchPage {

    protected int from;
    protected int size;

    public SearchPage(int from, int size) {
        this.from = from;
        this.size = size;
    }

    public Integer getFrom() {
        return from;
    }

    public Integer getTo() {
        return from + size;
    }

    public SearchPage next() {
        this.from += size;
        return this;
    }
}
