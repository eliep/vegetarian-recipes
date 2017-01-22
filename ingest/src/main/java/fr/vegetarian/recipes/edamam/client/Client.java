package fr.vegetarian.recipes.edamam.client;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.RuntimeJsonMappingException;
import fr.vegetarian.recipes.Ingest;
import fr.vegetarian.recipes.edamam.cache.CacheInterface;
import fr.vegetarian.recipes.edamam.response.SearchResponse;

import java.io.*;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;

public class Client {

    public static final String SEARCH_RECIPES_URI = "https://api.edamam.com/search" ; //"http://localhost:9200/recipes/_search"
    public static final String APP_ID_PARAMETER = "appId" ;
    public static final String APP_KEY_PARAMETER = "appKEY" ;

    protected String appId;
    protected String appKey;
    protected CacheInterface cache = null;

    public Client(String appId, String appKey) {
        this.appId = appId;
        this.appKey = appKey;

    }

    public String getAppId() {
        return appId;
    }

    public Client setAppId(String appId) {
        this.appId = appId;
        return this;
    }

    public String getAppKey() {
        return appKey;
    }

    public Client setAppKey(String appKey) {
        this.appKey = appKey;
        return this;
    }

    public Client setCache(CacheInterface cache) {
        this.cache = cache;
        return this;
    }

    public Optional<SearchResponse> search(SearchParameter parameter) throws IOException, RuntimeException {
        Optional<SearchResponse> maybeResponse;
        Optional<String> maybeResult = Optional.empty();

        boolean resultFromCache = false;
        if (cache != null) {
            maybeResult = cache.get(parameter);
            resultFromCache = maybeResult.isPresent();
        }

        if (!maybeResult.isPresent()) {
            Map<String, String> urlParameters = parameter.getUrlParameters();
//            urlParameters.put(APP_ID_PARAMETER, getAppId());
//            urlParameters.put(APP_KEY_PARAMETER, getAppKey());
            GetRequest request = new GetRequest(SEARCH_RECIPES_URI, urlParameters);
            maybeResult = request.execute();
        }

        if (cache != null && !resultFromCache && maybeResult.isPresent()) {
            cache.set(parameter, maybeResult.get());
        }

        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);

        maybeResponse = maybeResult.map((result) -> {

            SearchResponse response = null;
            try {
                response = objectMapper.readValue(result, SearchResponse.class);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            return response;
        });

        return maybeResponse;
    }
}
