#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D2        // D2 (GPIO4)
#define DHTTYPE DHT11
#define RELAY_PIN D1     // D1 (GPIO5)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();

  // Set relay pin as output
  pinMode(RELAY_PIN, OUTPUT);

  // Initially turn off the relay
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  // Read humidity
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again)
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print humidity to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Turn on the fan if humidity is above 75%
  if (humidity > 75) {
    digitalWrite(RELAY_PIN, LOW);  // Turn the fan on
    Serial.println("Fan is ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Turn the fan off
    Serial.println("Fan is OFF");
  }

  // Wait a few seconds between measurements
  delay(2000);
}