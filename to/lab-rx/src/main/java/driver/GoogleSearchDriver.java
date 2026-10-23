package driver;

import app.CrawlerApp;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.List;
import java.util.stream.StreamSupport;

public class GoogleSearchDriver {

    public static final String USER_AGENT = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.95 Safari/537.11";

    public static List<String> searchForImages(String query) throws IOException, InterruptedException {
        var htmlQuery = query.replace(' ', '+');

        var searchSource = downloadUrlSource(htmlQuery);
        return extractPhotoUrls(searchSource);
    }

    private static String downloadUrlSource(String searchQuery) throws IOException, InterruptedException {
        var client = HttpClient.newHttpClient();
        HttpResponse<String> response = sendRequest(client, searchQuery);
        return response.body();
    }

    private static HttpResponse<String> sendRequest(HttpClient client, String query) throws IOException, InterruptedException {
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("https://google.serper.dev/images"))
                .header("X-API-KEY", CrawlerApp.SERPER_API_KEY)
                .header("Content-Type", "application/json")
                .POST(HttpRequest.BodyPublishers.ofString("""
                {"q":"%s"}
                """.formatted(query)))
                .build();

        return client.send(
                request,
                HttpResponse.BodyHandlers.ofString()
        );
    }

    private static List<String> extractPhotoUrls(String responseJson) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        JsonNode rootNode = objectMapper.readTree(responseJson);
        JsonNode items = rootNode.get("images");

        if (items == null) {
            throw new IllegalArgumentException("Invalid response format:\n" + responseJson);
        }

        return StreamSupport.stream(items.spliterator(), false)
                .map(itemNode -> itemNode.get("imageUrl").asText())
                .toList();
    }
}
