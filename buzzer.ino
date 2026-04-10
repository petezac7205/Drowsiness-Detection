#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "cheese";
const char* password = "turnipip";

// Replace with your Flask server IP addres
const char* serverURL = "http://192.168.16.105:5000/status";  

#define BUZZER_PIN 4  // GPIO 4

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);

    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      payload.trim();  // ✅ Trim whitespace
      Serial.println("📥 Response: " + payload);

      if (payload == "1") {
        Serial.println("😴 Drowsiness Detected! Buzzing...");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
      } else {
        Serial.println("🙂 Status OK. No drowsiness.");
      }
    } else {
      Serial.printf("❌ Error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("📶 WiFi not connected");
  }

  delay(10000); // Poll every 10 seconds
}
