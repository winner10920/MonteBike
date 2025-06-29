#include "ArduinoLowPower.h"
#include <SCServo.h>
#include <LSM6DS3.h>
#include <Wire.h>
#include <SilabsMicrophoneAnalog.h>
#include "BLEStuff.h"
#include "ServoStuff.h"
#include "AccStuff.h"
#include "MicStuff.h"

bool externalAntenna = false;
bool enableGyro = false;

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);

  if(enableGyro) enableAcc();
  if(externalAntenna) enableExternalAntenna();
  
  setupServo();
  
}

void loop() {

   
  runBLE();
  runServo();
}

