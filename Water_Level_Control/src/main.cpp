/**
 * Water Level Control
 * 
 * Sketch to control water pump in dehumidifier tank. Instead of the humidifier shutting off
 * when it gets too full, this device will turn on a 12V pump to pump the water out. The
 * pump will turn on when the water level reaches a high limit, then continue to pump until
 * the level reaches the low limit (i.e., two trigger points).
 * 
 * This will use a WAVGAT Arduino UNO clone, an HC-SR04 ultrasonic transducer to detect the
 * water level, and a relay to control the pump. I'll power the whole thing using an old PC
 * power supply (perfect, as it will supply both 5V for the WAVGAT and sensor, along with
 * 12V for the pump.
 * 
 * TODO: Currently, a test LED behaves correctly on the relay output, but if I connect the
 * actual relay I am seeing inconsistent behavior. I'm not sure if this is because the USB
 * port 5V power can't handle the relay load, or the output pin on the WAVGAT can't handle
 * the current required to drive the relay. To find out, my first step will be to add an
 * NPN emitter follower between the output pin and the relay. If that doesn't fix it, I'll
 * need to try external 5V power.
 * 
 * TODO: Right now I have fixed values for the hi/lo limits. I'd like to use small pots
 * to be able to adjust those in-place.
 * 
 * TODO: I still need to design and print a mount for the sensor!
 * 
 * TODO: The values I'm getting from the sensor seem to jump around a lot. I may try
 * increasing the trigger frequency and taking an average for the value.
 */
#include <Arduino.h>
#include <HCSR04.h>

const int RELAY_PIN = 10;
const float hiLimit = 15.0f;
const float loLimit = 5.0f;

UltraSonicDistanceSensor distanceSensor(13, 12);  // initialize a sensor on digital pins 13 (trigger) and 12 (echo)
float distance;

void checkDistance();
float getAverageDistance();
void relayTestLoop();
void sensorTestLoop();

void setup() {
  Serial.begin(9600);
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);     // Note: relay board pin is active low
  digitalWrite(RELAY_PIN, HIGH);  // turn off relay initially
}

void checkDistance() {
  if (distance <= loLimit) {
    digitalWrite(RELAY_PIN, LOW);   // water too high, turn on pump
  }
  else if (distance >= hiLimit) {
    digitalWrite(RELAY_PIN, HIGH);  // water low enough, turn off pump
  }
}

float getAverageDistance() {
  const int numSamples = 10;
  float runningTotal = 0.0;
  for (int i = 0; i < numSamples; i++) {
    runningTotal += distanceSensor.measureDistanceCm();
    delayMicroseconds(100);
  }
  return runningTotal / numSamples;
}

void loop() {
  sensorTestLoop();
  // relayTestLoop();
}

void sensorTestLoop() {
  // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
  // distance = distanceSensor.measureDistanceCm();
  distance = getAverageDistance();
  Serial.println(distance);
  checkDistance();
  delay(1000);
}

void relayTestLoop() {
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn on built-in LED
  digitalWrite(RELAY_PIN, HIGH);    // deactivate the relay
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn off built-in LED
  digitalWrite(RELAY_PIN, LOW);     // activate the relay
}