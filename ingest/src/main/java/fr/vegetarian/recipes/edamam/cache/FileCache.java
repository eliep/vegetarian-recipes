package fr.vegetarian.recipes.edamam.cache;

import fr.vegetarian.recipes.Ingest;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.Optional;

public class FileCache implements CacheInterface {

    protected String folder = null;

    public FileCache(String folder) throws IOException {
        if (!folder.endsWith("/"))
            folder += "/";

        this.folder = folder;
        boolean exist = false;
        try {
            exist = (!Files.isDirectory(Paths.get(folder)))
                    ? (new File(folder)).mkdirs()
                    : true;
        } catch (Exception e) {
            Ingest.getLogger().error("Unable to find or create " + folder + " directory", e);
        }

        if (!exist)
            throw new IOException("Directory " + folder + " doesn't exists and can't be created");
    }

    @Override
    public boolean exist(CacheKeyInterface keyObject) {
        File f = new File(getFullPathForKey(keyObject.getKey()));
        return (f.exists() && !f.isDirectory());
    }

    @Override
    public Optional<String> get(CacheKeyInterface keyObject) {
        if (!exist(keyObject))
            return Optional.empty();

        String value = null;
        try {
            Path file = Paths.get(getFullPathForKey(keyObject.getKey()));
            byte[] bytes = Files.readAllBytes(file);
            value = new String(bytes, "UTF-8");
        } catch (IOException e) {

        }

        return (value != null) ? Optional.of(value) : Optional.empty();
    }

    @Override
    public boolean set(CacheKeyInterface keyObject, String value) {

        boolean success = false;

        Path file = Paths.get(getFullPathForKey(keyObject.getKey()));
        try {
            byte[] bytes = value.getBytes("UTF-8");
            Files.write(file, bytes, StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING);
            success = true;
        } catch (Exception e) {
            Ingest.getLogger().error("Unable to write to "+getFullPathForKey(keyObject.getKey()), e);
        }

        return success;
    }

    protected String getFullPathForKey(String key) {
        return this.folder + key;
    }
}
