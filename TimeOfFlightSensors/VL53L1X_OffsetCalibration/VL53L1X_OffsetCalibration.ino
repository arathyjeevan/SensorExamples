/*
  Distance offset calibration for the VL53L1X sensor

  This is a modification of Armin Joachimsmeyer's example for SparkFun.
  This is a simplified and hopefully clarified version of that example.
  this version adds more feedback for the user, and has no architecture-specific
  #ifdefs.
  Be sure the vacuum tape has been removed from the front of the sensor.

  NOTE: you will need to power the sensor off and re-power it to clear the offset once it's set,
  or you can attach an output pin to the SHUTDOWN pin and the code will toggle it in the setup.

  Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

  The circuit:
  - VL53L1X SDA connected to SDA (A4)
  - VL53L1X SCL connected to SCL (A5)

  modified 21 July 2020
  by Tom Igoe
  based on Armin Joachimsmeyer's Sparkfun library example
*/

#include "SparkFun_VL53L1X.h"

SFEVL53L1X sensor;
const int shutDownPin = 2;


void setup(void) {
  Wire.begin();

  // initialize the digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  // stop program until Serial Monitor opens.
  // used for USB-native boards.
  // comment this out if running on an Uno:
  while (!Serial);

  // Print program name, and date and time of compilation:
  Serial.println("Sketch running on board: " __FILE__ "\ncompiled: " __DATE__ "    time: " __TIME__);

  Serial.println();
  Serial.println("*****************************************************************************************************");
  Serial.println("                                    Offset calibration");
  Serial.println("Place a light grey (17 % gray) target at a distance of 140mm in front of the VL53L1X sensor.");
  Serial.println("The calibration will start 5 seconds after a distance below 100 mm was detected for 1 second.");
  Serial.println("Use the resulting offset distance as parameter for the setOffset() function called after begin().");
  Serial.println("*****************************************************************************************************");
  Serial.println();

  // reset the sensor by taking the SHUTDOWN pin low, then high:
  pinMode(shutDownPin, OUTPUT);
  digitalWrite(shutDownPin, LOW);
  delay(5);
  digitalWrite(shutDownPin, HIGH);


  // begin() returns 0 on successful response from sensor, unlike other begin() functions:
  if (sensor.begin() != 0) {
    Serial.println("Sensor is not responding. Please check your wiring");
    while (true);
  }
  Serial.println("Sensor is working properly");

  Serial.println("\n\nPress enter to start reading.");
  // wait for a byte from the serial monitor:
  while (!Serial.available());


  // Short mode is appropriate for this example:
  sensor.setDistanceModeShort();
  sensor.setTimingBudgetInMs(50);

  // count of readings below 10cm:
  int lowDistanceReadings = 0;
  Serial.print("number of readings below 10cm: ");
  Serial.println(lowDistanceReadings);

  // stay in this while loop until you get 20 readings below 10cm:
  while (lowDistanceReadings < 20) {
    //initiate measurement:
    sensor.startRanging();
    // See if the sensor has a reading:
    while (!sensor.checkForDataReady());
    // get the distance in mm:
    int distance = sensor.getDistance();
    // clear the sensor's interrupt and turn off ranging:
    sensor.clearInterrupt();
    sensor.stopRanging();
    // get the ranging status error message:
    byte rangeStatus = sensor.getRangeStatus();
    // rangeStatus = 0 is a good reading:
    if (rangeStatus == 0) {
      // if it's less than 100mm (10cm):
      if ( distance < 100) {
        // increment the count:
        lowDistanceReadings++;
        Serial.print("readings below 10cm: ");
        Serial.println(lowDistanceReadings);
      } else {
        // start counting again
        lowDistanceReadings = 0;
        Serial.print("approximately ");
        Serial.println(distance);
        Serial.println(" uncalibrated reading > 100mm");
      }
    } else {
      // error from getRangeStatus():
      Serial.println("reading out of range");
      Serial.println("range Status message: ");
      Serial.println(rangeStatus);
    }
  }

  Serial.println("Distance below 10mm detected 20 times. Offset calibration will start in 5 seconds");
  Serial.println("Now place a target, 17% gray, at a distance of 140 mm from the sensor");
  delay(5000);
  Serial.println("Offset calibration starting now...");
  /*
       Place a target, 17 % gray, at a distance of 140 mm from the sensor and call the VL53L1X_CalibrateOffset () function.
       The calibration may take a few seconds. The offset correction is applied to the sensor at the end of calibration.

       The calibration function takes 50 measurements and then takes the difference between the target distance
       and the average distance and then calls setOffset() with this value.
  */
  Serial.print("Before calibration, offset =");
  Serial.println(sensor.getOffset());
  sensor.calibrateOffset(140);
  Serial.print("Result of offset calibration. offset =");
  Serial.print(sensor.getOffset());
  Serial.print(" mm");
  Serial.println();

  // measure periodically. Intermeasurement period must be >/= timing budget.
  sensor.setIntermeasurementPeriod(sensor.getTimingBudgetInMs() * 2);
  sensor.startRanging(); // Start once
  delay(5000);
}

void loop() {

  // initiate measurement:
  sensor.startRanging();
  // See if the sensor has a reading:
  while (!sensor.checkForDataReady());
  // get the distance in mm:
  int distance = sensor.getDistance();
  // clear the sensor's interrupt and turn off ranging:
  sensor.clearInterrupt();
  sensor.stopRanging();

  // if you didn't get a good reading, exit the loop:
  if (sensor.getRangeStatus() != 0) return;
  // print distance in mm:
  Serial.print(distance);
  Serial.println(" mm");
}
