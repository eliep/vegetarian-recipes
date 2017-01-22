package fr.vegetarian.recipes.edamam.client;

import fr.vegetarian.recipes.Ingest;
import org.apache.http.client.ClientProtocolException;

import javax.swing.text.html.Option;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.Map;
import java.util.Optional;

public class GetRequest {

    private String uri;
    private Map<String, String> parameters;

    public GetRequest(String uri, Map<String, String> parameters) {
        this.uri = uri;
        this.parameters = parameters;
    }

    protected static String queryString(Map<String, String> parameters) {
        return (parameters != null)
            ? "?" + parameters.entrySet().stream()
                .map( (e) ->  e.getKey()+"="+e.getValue())
                .reduce("", (p1, p2) -> p1+"&"+p2)
            : "";
    }

    public Optional<String> execute() throws IOException {
        String stringUrl = uri + queryString(parameters);
        Optional<String> maybeResult = Optional.empty();

        URL url = new URL(stringUrl);
        HttpURLConnection request = (HttpURLConnection) url.openConnection();
        request.setRequestMethod("GET");
        maybeResult = Optional.of(readResult(request));

        return maybeResult;
    }

    private static String readResult(HttpURLConnection request) throws IOException {

        Integer responseCode = request.getResponseCode();
        if (responseCode != HttpURLConnection.HTTP_OK) {
            String error = "";
            try {
                error = readBuffer(request.getErrorStream());
            } catch (IOException e) { }

            throw new IOException(String.format("Call to %s fail with error code %d and error message %s", request.getURL().toString(), responseCode, error));
        }

        return readBuffer(request.getInputStream());

    }

    private static String readBuffer(InputStream inputStream) throws IOException {

        BufferedReader in = new BufferedReader(new InputStreamReader(inputStream));

        StringBuffer response = new StringBuffer();
        int BUFFER_SIZE=1024;
        char[] buffer = new char[BUFFER_SIZE];
        int charsRead = 0;

        while ( (charsRead  = in.read(buffer, 0, BUFFER_SIZE)) != -1) {
            response.append(buffer, 0, charsRead);
        }

        in.close();
        return response.toString();
    }
}
