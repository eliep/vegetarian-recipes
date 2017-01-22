package fr.vegetarian.recipes;

import com.fasterxml.jackson.databind.ObjectMapper;
import fr.vegetarian.recipes.data.RecipeData;
import fr.vegetarian.recipes.data.RecipeDataset;
import fr.vegetarian.recipes.config.EdamamApiConfig;
import fr.vegetarian.recipes.edamam.cache.CacheInterface;
import fr.vegetarian.recipes.edamam.cache.FileCache;
import fr.vegetarian.recipes.config.Config;
import fr.vegetarian.recipes.config.ConfigLoader;
import fr.vegetarian.recipes.edamam.client.Client;
import fr.vegetarian.recipes.edamam.client.SearchParameter;
import fr.vegetarian.recipes.edamam.response.Hit;
import fr.vegetarian.recipes.edamam.response.SearchResponse;
import fr.vegetarian.recipes.elasticsearch.Indexer;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class Ingest {
    private final static String LOGGER_NAME = "fr.vegetarian.recipes.main.logger";
    private final static String CONFIG_FILENAME = "config.json";

    public static void main(String[] args) throws IOException {
        Config config = new ConfigLoader()
                .load(CONFIG_FILENAME)
                .orElseThrow(() -> new IOException());

        Client edamamClient = new Client(config.edamamApi.appId, config.edamamApi.appKey);
        CacheInterface cache = null;
        try {
            if (config.cache != null && config.cache.folder != null)
                cache = new FileCache(config.cache.folder);
        } catch (Exception e) {
            throw new IOException("Failed to configure cache, continue without caching");
        }
        if (cache != null)
            edamamClient.setCache(cache);


        ObjectMapper mapper = new ObjectMapper();
        RecipeDataset recipesDataset = new RecipeDataset();
        config.edamamApi.query.parallelStream()
                .map(term -> {
                    SearchParameter parameter = buildSearchParameter(term, config.edamamApi);
                    List<Hit> termResult = searchTerm(edamamClient, parameter);
                    return termResult;
                })
                .flatMap(termResult -> termResult.stream())
                .forEach(hit -> {
                    RecipeData
                        .buildFromRecipe(hit.recipe, RecipeData.RecipeFeatureType.NUTRIENT_DAILY, mapper)
                        .ifPresent(recipesDataset::add);
                });

        Indexer indexer = new Indexer(config.elasticsearch);
        indexer
            .init()
            .index(recipesDataset)
            .close();

    }

    public static SearchParameter buildSearchParameter(String query, EdamamApiConfig edamamApi) {
        return new SearchParameter.Builder()
                .diet(edamamApi.diet)
                .health(edamamApi.health)
                .query(query)
                .page(0, edamamApi.querySize)
                .build();
    }

    public static List<Hit> searchTerm(Client edamamClient, SearchParameter parameter) {
        List<Hit> result = new ArrayList<Hit>();
        Optional<SearchResponse> maybeResponse;
        do {
            maybeResponse = Optional.empty();
            try {
                getLogger().debug(parameter);
                maybeResponse = edamamClient.search(parameter);
                maybeResponse.ifPresent(response -> {
                    result.addAll(response.hits);
                });

                parameter.nextPage();

            } catch (IOException e) {
                getLogger().error("Connection failed", e);

            } catch (RuntimeException e) {
                getLogger().error("Deserialization failed", e);
            }
        } while (maybeResponse.isPresent() && maybeResponse.get().hasNext());

        return result;
    }

    public static Logger getLogger() {
        return LogManager.getLogger(Ingest.LOGGER_NAME);
    }
}
