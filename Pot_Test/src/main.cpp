/**
 * Test reading values from an adjustment potentiometer on a WAVGAT UNO board.
 * 
 * Notes:
 * With the pot I chose, I get values from around 0 to 4K. I want to map these to
 * distances from a ultrasonic sensor. I want the distance values to range from
 * 
 */
#include <Arduino.h>

const int potPin = A0;
int adjVal = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  adjVal = analogRead(potPin);    // read the value from the sensor
  Serial.println(adjVal);
  delay(1000);
  // put your main code here, to run repeatedly:
}