
#include "config.h"
#include <NewPing.h>

#define TRIGGER_PIN 15    // Arduino pin tied to trigger pin on the ultrasonic sensor. D8 pin from ESP8266 ,D15 Esp32
#define ECHO_PIN 13       // Arduino pin tied to echo pin on the ultrasonic sensor. D7 pin from EESP8266 , D13
#define MAX_DISTANCE 200  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.




// setting up the 'MyFarmSensor' and 'counter' feed
AdafruitIO_Feed *MyFarmSensor = io.feed("MyFarmSensor");
AdafruitIO_Feed *counter = io.feed("counter");

int count = 0; // declaring the variable called count and assigning it to 0

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (!Serial)
    ;

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  MyFarmSensor->onMessage(handleMessage); // handle if any incoming message from this feed (Not required)

  // wait for a connection to WiFi
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  MyFarmSensor->get(); // not required
}

void loop() {
  io.run();

  Serial.print("Ping: ");
  int sensor_value = sonar.ping_cm();
  Serial.print(sensor_value);  // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  if (sensor_value < 10 && sensor_value > 0 ) {  // detecing if somone crossed or not
    Serial.print("inside");
    MyFarmSensor->save("Somebody has crossed the fence"); // Command to push the value to the feed MyFrmSensor
    delay(3000);
    count++;
    MyFarmSensor->save("Its safe"); 
    counter->save(count);  //Command to push the value to the feed counter
    delay(1000);
  } 
  else {
  }
}


void handleMessage(AdafruitIO_Data *data) { // not required

  // Serial.print("received <- ");
  // Serial.println(data->value());
}
