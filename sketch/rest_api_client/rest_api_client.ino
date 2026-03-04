#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* API_URL =
"https://api.openweathermap.org/data/2.5/forecast?lat=6.12&lon=106.49&appid=4997ec727dd6b0fbf3246663f400abc1&units=metric";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(API_URL);

    int httpCode = http.GET();

    if (httpCode > 0) {

      StaticJsonDocument<8192> doc;

      DeserializationError error =
      deserializeJson(doc, http.getStream());

      if (error) {
        Serial.println("JSON parse gagal");
        return;
      }

      // ambil data forecast pertama
      float suhu = doc["list"][0]["main"]["temp"];
      int humidity = doc["list"][0]["main"]["humidity"];
      const char* cuaca = doc["list"][0]["weather"][0]["main"];

      Serial.print("Suhu: ");
      Serial.println(suhu);

      Serial.print("Humidity: ");
      Serial.println(humidity);

      Serial.print("Cuaca: ");
      Serial.println(cuaca);

    }

    http.end();
  }

  delay(10000);
}