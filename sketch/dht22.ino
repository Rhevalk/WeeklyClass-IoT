#include "DHT.h"

#define DHTPIN 15       // pin data DHT22
#define DHTTYPE DHT22  // tipe sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT22 Test!");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); 

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | ");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | ");

  delay(2000);
}

