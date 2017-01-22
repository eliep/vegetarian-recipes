package fr.vegetarian.recipes.elasticsearch;

import com.fasterxml.jackson.databind.ObjectMapper;
import fr.vegetarian.recipes.data.RecipeData;
import fr.vegetarian.recipes.data.RecipeDataset;
import fr.vegetarian.recipes.Ingest;
import fr.vegetarian.recipes.config.ElasticsearchConfig;
import org.apache.commons.lang3.StringUtils;
import org.elasticsearch.action.bulk.BulkRequestBuilder;
import org.elasticsearch.action.bulk.BulkResponse;
import org.elasticsearch.action.index.IndexRequestBuilder;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.transport.client.PreBuiltTransportClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.util.stream.Collectors;

public class Indexer {

    public static final String RECIPE_MAPPING_FILENAME = "recipe_mapping.json";
    public static final String RECIPE_TYPE_NAME = "recipe";
    public static final String RECIPE_INDEX_NAME = "recipes";

    private ElasticsearchConfig esConfig;
    private TransportClient esClient;
    private String mapping;
    ObjectMapper mapper;


    public Indexer(ElasticsearchConfig esConfig) throws IOException {
        this.esConfig = esConfig;
        this.mapping = loadMapping();
    }

    private String loadMapping() throws IOException {
        ClassLoader classLoader = getClass().getClassLoader();
        URL configResource = classLoader.getResource(RECIPE_MAPPING_FILENAME);
        if (configResource == null)
            configResource = classLoader.getResource("/" + RECIPE_MAPPING_FILENAME);

        return StringUtils.join(
                new BufferedReader(new InputStreamReader(configResource.openStream(), StandardCharsets.UTF_8))
                        .lines()
                        .collect(Collectors.toList()), "\n");
    }

    public Indexer init() throws UnknownHostException {
        InetSocketTransportAddress address = new InetSocketTransportAddress(
                InetAddress.getByName(esConfig.hostname),
                Integer.valueOf(esConfig.port));

        esClient = new PreBuiltTransportClient(Settings.EMPTY)
                .addTransportAddress(address);

        boolean exists = esClient.admin().indices()
                .prepareExists(RECIPE_INDEX_NAME)
                .execute().actionGet().isExists();

        if (exists)
            esClient.admin().indices().prepareDelete(RECIPE_INDEX_NAME).get();


        esClient.admin().indices().prepareCreate(RECIPE_INDEX_NAME)
                .setSettings(Settings.builder()
                        .put("index.number_of_shards", 1)
                        .put("index.number_of_replicas", 1)
                )
                .addMapping(RECIPE_TYPE_NAME, mapping)
                .get();


        return this;
    }

    public Indexer index(RecipeDataset recipes) {
        BulkRequestBuilder bulkRequest = esClient.prepareBulk();
        for (RecipeData recipe: recipes.getRecipes().values()) {
            IndexRequestBuilder toIndex = esClient.prepareIndex(RECIPE_INDEX_NAME, RECIPE_TYPE_NAME, recipe.getId())
                    .setSource(recipe.toString());
            bulkRequest.add(toIndex);
        }
        BulkResponse bulkResponse = bulkRequest.get();
        if (bulkResponse.hasFailures()) {
            Ingest.getLogger().error("fail to index:\n"+bulkResponse.buildFailureMessage());
        }
        return this;
    }

    public void close() {
        esClient.close();
    }
}
