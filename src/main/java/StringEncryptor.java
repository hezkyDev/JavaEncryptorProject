import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.HttpURLConnection;

public class StringEncryptor {

    // Native method declaration for encryption
    public native String encryptString(String input);

    static {
        // Load the native library
        System.loadLibrary("NativeHashingEncryption");
    }

    public static void main(String[] args) throws Exception {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        System.out.println("Enter the string to hash and encrypt:");
        String input = reader.readLine();

        // Check for empty string and handle it
        if (input == null || input.trim().isEmpty()) {
            System.out.println("Error: Input string cannot be empty.");
            return;
        }

        // Download the public key from the URL
        String publicKey = getPublicKey();
        System.out.println("Public Key: " + publicKey);

        // Call the native method to hash and encrypt the input string
        StringEncryptor encryptor = new StringEncryptor();
        String encryptedHash = encryptor.encryptString(input);

        System.out.println("Encrypted Hash: " + encryptedHash);
    }

    // Method to download public key from URL
    private static String getPublicKey() throws Exception {
        URL url = new URL("https://demo.api.piperks.com/.well-known/pi-xcels.json");
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");

        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuilder content = new StringBuilder();

        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }

        in.close();
        return content.toString();
    }
}
