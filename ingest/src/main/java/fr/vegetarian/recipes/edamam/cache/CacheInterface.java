package fr.vegetarian.recipes.edamam.cache;

import java.util.Optional;

public interface CacheInterface {

    boolean exist(CacheKeyInterface keyObject);

    Optional<String> get(CacheKeyInterface keyObject);

    boolean set(CacheKeyInterface keyObject, String value);
}
