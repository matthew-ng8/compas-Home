/*

*/



#include <Arduino_HTS221.h>
#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>
#define timeSeconds 5


BLEService sensorService("19B10010-E8F2-537E-4F6C-D104768A1999");
//BLEIntCharacteristic tempCharac("19B10010-E8F2-537E-4F6C-D104768A1909", BLENotify |BLERead);
BLEFloatCharacteristic tempCharac("19B10010-E8F2-537E-4F6C-D104768A1909", BLENotify |BLERead);


BLEIntCharacteristic humidCharac("19B10010-E8F2-537E-4F6C-D104768A1999", BLENotify| BLERead);
int inPin = 12;//on the board
int val;
// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;



void detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  startTimer = true;
  lastTrigger = millis();
}


void setup() {

  Serial.begin(9600);
  while (!Serial);

  //BLE STUFF
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

  //initialize Humidity and sensor stuff
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  
  pinMode(inPin, INPUT);//setting the inPin to be input
  val = 0;
  //attachInterrupt(digitalPinToInterrupt(inPin), detectsMovement, RISING);

  //initialize the gesture, color, proximity sensor
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  

}



void loop() {
  int proximity = 0;
  int r = 0, g = 0, b = 0;
  unsigned long lastUpdate = 0;
   if(BLE.connected()){
      Serial.println("IM  CONNECTED"); 
   }
  else{
    BLE.disconnect();
    Serial.println("IM NOT CONNECTED :( ");
  }

  if(BLE.central()){
    Serial.print("connected to central");
    Serial.println(BLE.central().address());
  }
  else{
    Serial.println("NO CENTRAL CONNECTED");
  }
  // read all the sensor values

  float temperature = HTS.readTemperature();

  float humidity    = HTS.readHumidity();


  // print each of the sensor values
  /*
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  */
  tempCharac.writeValue(temperature);//uncomment to test FloatCharacteristic

  /*
  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");
  */

  humidCharac.writeValue(humidity);
  // print an empty line

  Serial.println();
  pinMode(inPin, INPUT);
  val = digitalRead(inPin);
  Serial.print(val);
  Serial.println(" ");
  if(val == 0){
    Serial.println("NO MOTION");
  }
  else if(val == 1){
    Serial.println("Motion");
  }

  /*
  now = millis();
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped...");
    startTimer = false;
  }
  */

// Check if a proximity reading is available.
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();
  }

  // check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;

      default:
        // ignore
        break;
    }
  }

   if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b);
  }

  if (millis() - lastUpdate > 100) {
    lastUpdate = millis();
    Serial.print("PR=");
    Serial.print(proximity);
    Serial.print(" rgb=");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.println(b);
  }

  // wait 1 second to print again

  delay(1000);

}
