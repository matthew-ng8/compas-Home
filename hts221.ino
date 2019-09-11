/*

  HTS221 - Read Sensors



  This example reads data from the on-board HTS221 sensor of the

  Nano 33 BLE Sense and prints the temperature and humidity sensor

  values to the Serial Monitor once a second.



  The circuit:

  - Arduino Nano 33 BLE Sense



  This example code is in the public domain.

*/



#include <Arduino_HTS221.h>
#include <ArduinoBLE.h>

BLEService sensorService("19B10010-E8F2-537E-4F6C-D104768A1999");
BLEFloatCharacteristic tempCharac("19B10010-E8F2-537E-4F6C-D104768A1999", BLEBroadcast | BLEWriteWithoutResponse);
BLEFloatCharacteristic humidCharac("19B10010-E8F2-537E-4F6C-D104768A1999", BLEBroadcast | BLEWriteWithoutResponse);





void setup() {

  Serial.begin(9600);

  while (!Serial);

 if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  BLE.setLocalName("FerdmanSensor");
  BLE.setAdvertisedService(sensorService); //advertises the service we made
  sensorService.addCharacteristic(tempCharac);
  sensorService.addCharacteristic(humidCharac);
  BLE.addService(sensorService);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");

  if (!HTS.begin()) {

    Serial.println("Failed to initialize humidity temperature sensor!");

    while (1);

  }

}



void loop() {

  
  // read all the sensor values

  float temperature = HTS.readTemperature();

  float humidity    = HTS.readHumidity();


  // print each of the sensor values

  Serial.print("Temperature = ");

  Serial.print(temperature);

  Serial.println(" °C");

  tempCharac.writeValue(temperature);

  Serial.print("Humidity    = ");

  Serial.print(humidity);

  Serial.println(" %");



  // print an empty line

  Serial.println();



  // wait 1 second to print again

  delay(1000);

}
