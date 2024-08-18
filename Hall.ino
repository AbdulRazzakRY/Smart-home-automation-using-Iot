#define BLYNK_TEMPLATE_ID "TMPL36Lk4XJRe"
#define BLYNK_TEMPLATE_NAME "Hall and Room"
#define BLYNK_AUTH_TOKEN "5JHXXYPUTXX8Qw7M2oPcS5-vpfYjynqz"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D3     // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11 

#define PIR1PIN D1    // Pin where the first PIR sensor is connected
#define PIR2PIN D2    // Pin where the second PIR sensor is connected

DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN; // Your Blynk Auth Token
char ssid[] = "OnePlus 9R";       // Your WiFi SSID
char pass[] = "123454321"; // Your WiFi Password

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(PIR1PIN, INPUT);
  pinMode(PIR2PIN, INPUT);
}

void loop() {
  Blynk.run();

  // Reading temperature and humidity from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Reading motion from PIR sensors
  bool motionDetected1 = digitalRead(PIR1PIN);
  bool motionDetected2 = digitalRead(PIR2PIN);

  // Sending the results to Blynk
  Blynk.virtualWrite(V0, motionDetected1);
  Blynk.virtualWrite(V1, motionDetected2);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);

  // Printing the results to Serial
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, PIR1: ");
  Serial.print(motionDetected1 ? "Motion detected" : "No motion");
  Serial.print(", PIR2: ");
  Serial.println(motionDetected2 ? "Motion detected" : "No motion");

  // Wait a few seconds between measurements.
  delay(2000);
}