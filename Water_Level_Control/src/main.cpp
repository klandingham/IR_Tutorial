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
 * Note: values for the relay control pin assume a driver transistor in the circuit (emitter
 * follower) to take load off output pin, so value is inverted.
 * 
 * TODO: Right now I have fixed values for the hi/lo limits. I'd like to use small pots
 * to be able to adjust those in-place.
 * 
 * TODO: I still need to design and print a mount for the sensor!
 */
#include <Arduino.h>
#include <HCSR04.h>

// dug
const int RELAY_PIN = 10;
const float hiLimit = 15.0f;
const float loLimit = 5.0f;

UltraSonicDistanceSensor distanceSensor(13, 12);  // initialize a sensor on digital pins 13 (trigger) and 12 (echo)
float distance;

void checkDistance();
void relayTestLoop();
void sensorTestLoop();

void setup() {
  Serial.begin(9600);
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);     // Note: relay board pin is active low
  digitalWrite(RELAY_PIN, LOW);  // turn off relay initially
}

void checkDistance() {
  /* if distance is beyond sensor range it will be negative - just
   * ensure pump is off in these cases
   */
  if (distance < 0.0)
    digitalWrite(RELAY_PIN, LOW);   // distance out of range, turn off pump
  else if (distance <= loLimit)
    digitalWrite(RELAY_PIN, HIGH);  // water too high, turn on pump
  else if (distance >= hiLimit)
    digitalWrite(RELAY_PIN, LOW);   // water low enough, turn off pump
}

void loop() {
  sensorTestLoop();
  // relayTestLoop();
}

void sensorTestLoop() {
  // Every 500 milliseconds, do a measurement using the sensor and print the distance in centimeters.
  distance = distanceSensor.measureDistanceCm();
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