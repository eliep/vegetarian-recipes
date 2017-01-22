package fr.vegetarian.recipes.config;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import fr.vegetarian.recipes.Ingest;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.net.URL;
import java.util.Optional;

public class ConfigLoader {

    public static final Logger logger = Ingest.getLogger();

    public Optional<Config> load(String configFileName) {
        ClassLoader classLoader = getClass().getClassLoader();
        URL configResource = classLoader.getResource(configFileName);
        if (configResource == null) {
            configResource = classLoader.getResource("/" + configFileName);
        }

        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.configure(JsonParser.Feature.ALLOW_COMMENTS, true);
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);

        Config config = null;
        try {
            InputStream stream = configResource.openStream();
            config = objectMapper.readValue(stream, Config.class);
            stream.close();

        } catch (FileNotFoundException e) {
            logger.error("Unable to load configuration file", e);

        } catch (JsonParseException|JsonMappingException e) {
            logger.error("Unable to parse configuration file: ", e);

        } catch (IOException e) {
            logger.error("IOException while accessing configuration file", e);
        }

        return (config != null) ? Optional.of(config) : Optional.empty();

    }

}
