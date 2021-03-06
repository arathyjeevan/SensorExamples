/*
  AS7262 color sensor

  Reads light readings and temperature readings from AMS AS7262 sensor.
  Uses Sparkfun's AS726x library. Works with both Sparkfun and Adafruit 
  breakout boards.

  Datasheet: https://cdn.sparkfun.com/assets/parts/1/2/2/4/9/AS7262_Datasheet.pdf

   Visible wavelengths:
   450nm: violet
   500nm: blue
   550nm: green
   570nm: yellow
   600nm: orange
   650nm: red

   According to the datasheet, all channels tested under 5700K White LED light.

   based on Sparkfun libraries
   modified 16 Feb 2020
   by Tom Igoe
*/

#include <AS726X.h>

// make an instance of the color sensor library:
AS726X colorSensor;

void setup() {
  // initialize serial and I2C communication:
  Serial.begin(9600);
  Wire.begin();
  
  // initialize sensor:
  colorSensor.begin();

  // optional sensor settings:
  //Set the measurement mode
  // 0: Continuous reading of VBGY
  // 1: Continuous reading of GYOR
  // 2: Continuous reading of all channels (default)
  // 3: One-shot reading of all channels
  colorSensor.setMeasurementMode(2);

  // Integration time = time to get good readings.
  // number is in units of  2.8ms.
  // for measurement mode 2 or 3, you need 280ms, or integrationTime(100)
  // default is 50:
  colorSensor.setIntegrationTime(100);

  // gain sets sensitivity:
  // 0: 1x (default)
  // 1: 3.7x
  // 2: 16x
  // 3: 64x
  colorSensor.setGain(3);
}

void loop() {
  // read sensor:
  colorSensor.takeMeasurements();

  // get results:
  // for uncalibrated readings use .getViolet(), .getBlue(), etc.
  // uncalibrated readings are ints, not floats.
  float cViolet = colorSensor.getCalibratedViolet();
  float cBlue = colorSensor.getCalibratedBlue();
  float cGreen = colorSensor.getCalibratedGreen();
  float cYellow = colorSensor.getCalibratedYellow();
  float cOrange = colorSensor.getCalibratedOrange();
  float cRed = colorSensor.getCalibratedRed();
  String readings = "violet: ";
  readings += String(cViolet);
  readings += ", blue:";
  readings += String(cBlue);
  readings += ", green:";
  readings += String(cGreen);
  readings += ", yellow: ";
  readings += String(cYellow);
  readings += ", orange:";
  readings += String(cOrange);
  readings += ", red:";
  readings += String(cRed);
  
  // get temperature in degrees C (for degrees F, use float .getTemperatureF())
  readings += ", temp.: ";
  readings += String(colorSensor.getTemperature());

  // print results:
  Serial.println(readings);
}
