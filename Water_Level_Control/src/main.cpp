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
 * TODO: I still need to design and print a mount for the sensor!
 * TODO: Save circuit in Fritzing.
 */
#include <Arduino.h>
#include <HCSR04.h>

// #define DEBUG 1

// Water level adjustment ranges in centimeters
#define MAX_HIGH_WATER_LEVEL 2
#define MIN_HIGH_WATER_LEVEL 10
#define MAX_LOW_WATER_LEVEL 10
#define MIN_LOW_WATER_LEVEL 35

// Water level adjustment
const int loPotPin = A0;
const int hiPotPin = A1;
long readLoVal;
long readHiVal;
long highWaterLimit;
long lowWaterLimit;

const int RELAY_PIN = 10;
const float hiLimit = 15.0f;
const float loLimit = 5.0f;

UltraSonicDistanceSensor distanceSensor(13, 12);  // initialize a sensor on digital pins 13 (trigger) and 12 (echo)
float distance;

void checkDistance();
void readLimits();
void relayTestLoop();
void sensorTestLoop();

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
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
  else if (distance <= highWaterLimit)
    digitalWrite(RELAY_PIN, HIGH);  // water too high, turn on pump
  else if (distance >= lowWaterLimit)
    digitalWrite(RELAY_PIN, LOW);   // water low enough, turn off pump
}

void loop() {
  sensorTestLoop();
  // relayTestLoop();
}

/**
 * Reads high and low water level limits from the adjustment pots.
 */
void readLimits() {
  readLoVal = analogRead(loPotPin);
  highWaterLimit = map(readLoVal, 0L, 4095L, MAX_HIGH_WATER_LEVEL, MIN_HIGH_WATER_LEVEL);
  readHiVal = analogRead(hiPotPin);
  lowWaterLimit = map(readHiVal, 0L, 4095L, MAX_LOW_WATER_LEVEL, MIN_LOW_WATER_LEVEL);
#ifdef DEBUG
  Serial.print("High limit: ");
  Serial.print(highWaterLimit);
  Serial.print("\tLow limit: ");
  Serial.print(lowWaterLimit);
#endif
}

void sensorTestLoop() {
  readLimits();
  // Every 500 milliseconds, do a measurement using the sensor and print the distance in centimeters.
  distance = distanceSensor.measureDistanceCm();
#ifdef DEBUG
  Serial.println(distance);
#endif
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