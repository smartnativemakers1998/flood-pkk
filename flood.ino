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

// ====== Function to send message to Telegram ======
void sendToTelegram(String message) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
    http.begin(url);
    int httpCode = http.GET();
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  sendToTelegram("ESP Device is Online ✅ (Flood Detector with IR Sensor)");
}

void loop() {
  // ===== IR Sensor Reading =====
  int distance = random(2, 100);

  // ===== Sensor details kept in code =====
  String sensorDetails = "IR Sensor Distance: " + String(distance) + " cm";

  // ===== Status =====
  String message = "Sensor in-progress... 🔄";

  Serial.println("Sending to Telegram: " + message);
  Serial.println("Debug Sensor Values (Serial Only):");
  Serial.println(sensorDetails);
  Serial.println("------------------------------------------------");

  sendToTelegram(message);

  delay(10000); // send every 10 seconds
}
