#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <DHT11.h>

DHT11 dht11(2);

WebServer server(80);

// Mode AP
const char* ap_ssid = "ESP32-AP";
const char* ap_password = "12345678";

// variabel global
int temp = 0;
int hum = 0;


// Endpoint JSON
void handleSensor() {
  StaticJsonDocument<200> doc;

  dht11.readTemperatureHumidity(temp, hum);

  doc["temperature"] = temp;
  doc["humidity"] = hum;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// Endpoint root
void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 Sensor Dashboard</title>
    <style>
      body { font-family: Arial; text-align:center; margin-top:50px; }
      .data { font-size: 24px; margin: 10px; }
    </style>
  </head>
  <body>
    <h1>ESP32 Sensor Dashboard</h1>
    <div class="data">Temperature: <span id="temp">--</span> °C</div>
    <div class="data">Humidity: <span id="hum">--</span> %</div>

    <script>
      async function fetchData() {
        try {
          const res = await fetch('/sensor');
          const data = await res.json();
          document.getElementById('temp').textContent = data.temperature.toFixed(1);
          document.getElementById('hum').textContent = data.humidity.toFixed(1);
        } catch(err) {
          console.error(err);
        }
      }
      setInterval(fetchData, 1000);
      fetchData();
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point started!");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/sensor", HTTP_GET, handleSensor);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
