#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D4          // DHT11 data pin connected to GPIO2 (D4)
#define DHTTYPE DHT11      // DHT11 sensor

const char* ssid = "OPPO A31";         // Replace with your Wi-Fi SSID
const char* password = "12345678";     // Replace with your Wi-Fi password

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected to Wi-Fi");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature();  // In Celsius
    float humidity = dht.readHumidity();

    // Check if readings are valid
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("❌ Failed to read from DHT sensor!");
      delay(10);
      return;
    }

    WiFiClient client;
    HTTPClient http;

    String serverUrl = "http://192.168.43.83:3000/data";  // Your Node.js server
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = "{\"temperature\": ";
    jsonPayload += temperature;
    jsonPayload += ", \"humidity\": ";
    jsonPayload += humidity;
    jsonPayload += "}";

    int httpCode = http.POST(jsonPayload);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.print("✅ HTTP Response code: ");
      Serial.println(httpCode);
      Serial.println("📩 Server response: " + response);
    } else {
      Serial.print("❌ HTTP Request failed. Code: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("❌ WiFi not connected");
  }

  delay(10);  
}
