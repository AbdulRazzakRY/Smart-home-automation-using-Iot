[1:17 pm, 18/8/2024] PRAJWAL BVB: #define BLYNK_TEMPLATE_ID "TMPL3b0w-hPp-"
#define BLYNK_TEMPLATE_NAME "Terrace"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define TRIGGER_PIN 32       // GPIO pin connected to the ultrasonic sensor's trigger pin
#define ECHO_PIN 35          // GPIO pin connected to the ultrasonic sensor's echo pin
#define MAX_DISTANCE 200     // Maximum distance in centimeters
#define RAIN_SENSOR_PIN 34   // Analog pin connected to rain sensor

const float MAX_RAIN_LEVEL = 10.0;  // Maximum rain level in cm (adjust based on your sensor)
const int MAX_ANALOG_VALUE = 4095;  // Maximum analog value for ESP32 (12-bit ADC)

char auth[] = "kzfFpFOHjPT9FtLUjeWxuZyXOE4ApqOK"; // Your Blynk authentication token
char ssid[] = "OnePlus 9R";   // Your Wi-Fi network SSID
char pass[] = "123454321"; // Your Wi-Fi network password

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, pass);
    int wifiTimeout = 0;
    while (WiFi.status() != WL_CONNECTED && wifiTimeout < 60) {  // 30 seconds timeout
        delay(500);
        Serial.print(".");
        wifiTimeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
        Blynk.begin(auth, ssid, pass);
    } else {
        Serial.println("Failed to connect to WiFi");
    }

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RAIN_SENSOR_PIN, INPUT);
}

void loop() {
    Blynk.run();

    // Ultrasonic sensor distance measurement
    long duration, distance;
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH, MAX_DISTANCE * 58.2); // Timeout in microseconds
    distance = duration * 0.034 / 2;

    if (distance >= 0 && distance <= MAX_DISTANCE) {
        Serial.print("Distance: ");
        Serial.println(distance);
        Blynk.virtualWrite(V3, distance); // Send distance data to virtual pin V0
    } else {
        Serial.println("Out of range.");
        Blynk.virtualWrite(V3, 0); // Set to 0 if out of range
    }

    // Read rain level from sensor
    int analogValue = analogRead(RAIN_SENSOR_PIN);
    float rainLevelCm = (analogValue / (float)MAX_ANALOG_VALUE) * MAX_RAIN_LEVEL;  // Convert to rain level in cm
    Serial.print("Rain Level: ");
    Serial.print(rainLevelCm);
    Blynk.virtualWrite(V2, rainLevelCm); // Send rain level data to virtual pin V1

    Serial.println(" cm");  // Print the rain level in cm to the serial monitor
    
    delay(2000); // Delay between readings
}