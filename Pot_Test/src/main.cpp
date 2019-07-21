/**
 * Test reading values from an adjustment potentiometer on a WAVGAT UNO board.
 * 
 * Notes for the water level sensor:
 * With the pot I chose, I get values from around 0 to 4K. I want to map these to
 * distances from a ultrasonic sensor. For the high water limit (lowest distance),
 * I want an approximate range from 2 to 10 cm. For the low water limit, 10 to 30 cm.
 * 
 * Note: the "high limit" is the lowest distance value (water closest to the sensor),
 * and vice-versa.
 * 
 */
#include <Arduino.h>

#define DEBUG 1

/* All these are in centimeters */
#define MAX_HIGH_WATER_LEVEL 2
#define MIN_HIGH_WATER_LEVEL 10
#define MAX_LOW_WATER_LEVEL 10
#define MIN_LOW_WATER_LEVEL 35 

const int loPotPin = A0;
const int hiPotPin = A1;
long readLoVal;
long readHiVal;
long mappedLoVal;
long mappedHiVal;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  readLoVal = analogRead(loPotPin);    // read the value from the sensor
  mappedLoVal = map(readLoVal, 0L, 4095L, MAX_HIGH_WATER_LEVEL, MIN_HIGH_WATER_LEVEL);
  readHiVal = analogRead(hiPotPin);    // read the value from the sensor
  mappedHiVal = map(readHiVal, 0L, 4095L, MAX_LOW_WATER_LEVEL, MIN_LOW_WATER_LEVEL);
#ifdef DEBUG
  Serial.print("High limit: ");
  Serial.print(readLoVal);
  Serial.print(" -> ");
  Serial.print(mappedLoVal);
  Serial.print(", Low limit: ");
  Serial.print(readHiVal);
  Serial.print(" -> ");
  Serial.println(mappedHiVal);
#else
  Serial.print("High limit: ");
  Serial.print(mappedLoVal);
  Serial.print("\tLow limit: ");
  Serial.println(mappedHiVal);
#endif
  delay(1000);
  // put your main code here, to run repeatedly:
}