// ====== Select Board Type ======
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <HTTPClient.h>   // works for both ESP32 and ESP8266

// ====== WiFi Credentials ======
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== Telegram Bot Credentials ======
String botToken = "YOUR_BOT_TOKEN";  
String chatID   = "YOUR_CHAT_ID";    

// ====== Sensor Pins ======
const int irPin = 5;            // Example: GPIO5 for IR sensor (digital)
const int soilPin = 34;         // Example: GPIO34 for Soil Moisture (analog, ESP32 ADC pin)

// ====== Function to send message to Telegram ======
void sendToTelegram(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken +
                 "/sendMessage?chat_id=" + chatID +
                 "&text=" + message;
    http.begin(url);
    int httpCode = http.GET();
    http.end();
  }
}

void setup() {
  Serial.begin(115200);

  // Pin Modes
  pinMode(irPin, INPUT);

  // WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Send startup message
  sendToTelegram("ESP Device is Online ✅ (Flood Detector with Sensors)");
}

void loop() {
  // ===== IR Sensor Reading =====
  int irValue = digitalRead(irPin);
  String irStatus = (irValue == LOW) ? "Object Detected 🚨" : "No Object ✅";

  // ===== Soil Moisture Sensor Reading =====
  int soilValue = analogRead(soilPin);   // 0 (wet) → 4095 (dry)
  String soilStatus;
  if (soilValue < 1500) {
    soilStatus = "Soil is Wet 💧 (Possible Flood Risk)";
  } else if (soilValue < 3000) {
    soilStatus = "Soil is Moist 🌱";
  } else {
    soilStatus = "Soil is Dry ☀️";
  }

  // ===== Create Message =====
  String message = "🌊 Flood Detector Status:\n";
  message += "IR Sensor: " + irStatus + "\n";
  message += "Soil Moisture: " + soilStatus + "\n";
  message += "Raw Soil Value: " + String(soilValue);

  // ===== Send to Serial & Telegram =====
  Serial.println("Sending to Telegram:\n" + message);
  Serial.println("------------------------------------------------");

  sendToTelegram(message);

  delay(10000); // send every 10 seconds
}
