package fr.vegetarian.recipes.edamam.client;

import fr.vegetarian.recipes.Ingest;
import fr.vegetarian.recipes.edamam.cache.CacheKeyInterface;
import org.apache.commons.codec.digest.DigestUtils;

import java.util.HashMap;
import java.util.Map;

public class SearchParameter implements CacheKeyInterface, GetParametersInterface {
    public static final String QUERY_PARAMETER = "q";
    public static final String DIET_PARAMETER = "diet";
    public static final String HEALTH_PARAMETER = "health";
    public static final String FROM_PARAMETER = "from";
    public static final String TO_PARAMETER = "to";

    protected String query;
    protected String diet;
    protected String health;
    protected SearchPage page = null;

    public SearchParameter(Builder builder) {
        this.query = builder.query;
        this.diet = builder.diet;
        this.health = builder.health;
        this.page = builder.page;
    }

    public String getQuery() {
        return query;
    }

    public String getDiet() {
        return diet;
    }

    public String getHealth() {
        return health;
    }

    public SearchPage getPage() {
        return page;
    }

    public void nextPage() {
        this.page.next();
    }

    @Override
    public String getKey() {
        String key = GetRequest.queryString(getUrlParameters());
        return DigestUtils.md5Hex(key);
    }

    @Override
    public Map<String, String> getUrlParameters() {
        Map<String, String> parameters = new HashMap<>(4+2, 5);
        if (this.query != null)
            parameters.put(QUERY_PARAMETER, this.query);

        if (this.diet != null)
            parameters.put(DIET_PARAMETER, this.diet);

        if (this.health != null)
            parameters.put(HEALTH_PARAMETER, this.health);

        if (this.page != null) {
            parameters.put(FROM_PARAMETER, this.page.getFrom().toString());
            parameters.put(TO_PARAMETER, this.page.getTo().toString());
        }

        return parameters;
    }

    public String toString() {
        return GetRequest.queryString(getUrlParameters()) + " cached with key " + getKey();
    }

    public static class Builder {

        protected String query;
        protected String diet;
        protected String health;
        protected SearchPage page = null;

        public Builder() {

        }

        public Builder query(String query) {
            this.query = query;
            return this;
        }

        public Builder diet(String diet) {
            this.diet = diet;
            return this;
        }

        public Builder health(String health) {
            this.health = health;
            return this;
        }

        public Builder page(int from, int size) {
            this.page = new SearchPage(from, size);
            return this;
        }

        public SearchParameter build() {
            return new SearchParameter(this);
        }
    }

}
