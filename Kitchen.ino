#define BLYNK_TEMPLATE_ID "TMPL33mtBvBOz"
#define BLYNK_TEMPLATE_NAME "Kitchen"
#define BLYNK_AUTH_TOKEN "gaXjEcEinB-G7OI9BH8oclCltvcE53FA"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define FLAME_SENSOR_PIN 25   // GPIO pin connected to the flame sensor
#define SMOKE_SENSOR_PIN 34   // Analog pin connected to the smoke sensor
#define BUZZER_PIN 26         // GPIO pin connected to the buzzer
#define SMOKE_THRESHOLD 300   // Threshold value for smoke detection (adjust based on your sensor)
#define BUZZER_THRESHOLD 1800 // Threshold value to trigger the buzzer

char auth[] = BLYNK_AUTH_TOKEN; // Your Blynk authentication token
char ssid[] = "OnePlus 9R";     // Your Wi-Fi network SSID
char pass[] = "123454321";      // Your Wi-Fi network password

void setup() {
  Serial.begin(115200);        // Initialize serial communication
  pinMode(FLAME_SENSOR_PIN, INPUT);  // Set the flame sensor pin as input
  pinMode(BUZZER_PIN, OUTPUT);       // Set the buzzer pin as output

  // Connect to Wi-Fi and Blynk
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  // Flame sensor reading
  int flameDetected = digitalRead(FLAME_SENSOR_PIN);  // Read the digital value from the flame sensor

  if (flameDetected == LOW) {  // Flame detected (assuming active LOW)
    Serial.println("Flame detected!");
    Blynk.virtualWrite(V0, 1); // Send flame detected status to virtual pin V0
  } else {
    Serial.println("No flame detected.");
    Blynk.virtualWrite(V0, 0); // Send no flame status to virtual pin V0
  }

  // Smoke sensor reading
  int smokeLevel = analogRead(SMOKE_SENSOR_PIN);  // Read the analog value from the smoke sensor

  Serial.print("Smoke level: ");
  Serial.println(smokeLevel);
  Blynk.virtualWrite(V1, smokeLevel); // Send smoke level data to virtual pin V1

  if (smokeLevel > SMOKE_THRESHOLD) {  // Smoke detected
    Serial.println("Smoke detected!");
   // Blynk.virtualWrite(V1, 1); // Send smoke detected status to virtual pin V2
  } else {
    Serial.println("No smoke detected.");
    //Blynk.virtualWrite(V1, 0); // Send no smoke status to virtual pin V2
  }

  if (smokeLevel > BUZZER_THRESHOLD) {  // Trigger buzzer if smoke level exceeds BUZZER_THRESHOLD
    digitalWrite(BUZZER_PIN, HIGH);  // Turn the buzzer on
    Serial.println("Buzzer ON - High smoke level detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn the buzzer off
  }

  delay(1000);  // Wait for 1 second before reading again
}