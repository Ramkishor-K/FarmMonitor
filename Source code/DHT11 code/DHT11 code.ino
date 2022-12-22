#include "DHT.h"
#include "config.h"

#define DHTPIN 12  //D6 of ESP8266, D12 of ESP32

#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);


AdafruitIO_Feed *Humidity = io.feed("Humidity");
AdafruitIO_Feed *Temperature = io.feed("Temperature");

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  Humidity->get();
  Temperature->get();
}

void loop() {

  io.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  Humidity->save(h);
  Temperature->save(t);
  delay(2000);
}
