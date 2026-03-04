#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT
const char* mqtt_server = "broker.hivemq.com";
String KEY = "A7x9K2mQ";
WiFiClient espClient;
PubSubClient client(espClient);

// PIN
#define DHTPIN 4
#define DHTTYPE DHT22

#define GAS_PIN 34
#define RELAY_PIN 26
#define BUZZER_PIN 25

#define LED1 14
#define LED2 27
#define LED3 33

// DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variabel Global
float temperature = 0;
float humidity = 0;
float gasLevel_ppm = 0;

const int gasThreshold = 300;

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32_WOKWI")) {
      Serial.println("Connected");

      client.subscribe((KEY + "dashboard/led1").c_str());
      client.subscribe((KEY + "dashboard/led2").c_str());
      client.subscribe((KEY + "dashboard/led3").c_str());

    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void readSensors() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // kalibrasikan sesuai data min dan max
  // misalnya
  // 0.1 ppm = 843 (min)
  // 100K ppm = 4041 (max)
  int gasLevel_raw = analogRead(GAS_PIN);
  gasLevel_ppm = exp((0.00845 * gasLevel_raw) - 24.10);


  Serial.println("SENSOR DATA");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Hum : "); Serial.println(humidity);
  Serial.print("Gas : "); Serial.println(gasLevel_ppm);
}

void handleAlarm() {

  if (gasLevel_ppm > gasThreshold) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
    client.publish((KEY + "/alert/gas").c_str(), "GAS_BERBAHAYA");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }
}

void sendDataMQTT() {

  StaticJsonDocument<200> doc;

  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["gas"] = gasLevel_ppm;

  char buffer[256];
  serializeJson(doc, buffer);

  client.publish((KEY + "/sensor/data").c_str(), buffer);
}

void readDataMQTT(char* topic, byte* payload, unsigned int length) {

  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "dashboard/led1") {
    setLED(LED1, message);
  }

  if (String(topic) == "dashboard/led2") {
    setLED(LED2, message);
  }

  if (String(topic) == "dashboard/led3") {
    setLED(LED3, message);
  }
}

void setLED(int pin, String state) {
  if (state == "ON") {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  // WiFi Connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(readDataMQTT);
}

void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  readSensors();
  handleAlarm();
  sendDataMQTT();

  delay(3000);
}
