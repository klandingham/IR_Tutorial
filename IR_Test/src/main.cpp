#include <Arduino.h>
#include <IRremote.h>

// quick values: 1 = 0x2fd807f, 2 = 0x2fd40bf, 3 = 2fdc03f

// define sensor pin
const int RECV_PIN = 4;
const int DUMMY_VALUE = 5; // KIP: Added to test rebuild, delete when done

// define LED pins
const int redPin = 8;
const int yellowPin = 7;

// define integer to remember toggle state
int toggleState = 0;

// define IR receiver and results objects
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
    // serial monitor @ 9600 baud
    Serial.begin(9600);
    // enable IR receiver
    irrecv.enableIRIn();
    // set LED pins as outputs
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    Serial.println("Serial ready...");
}

void printRemoteValue(decode_results val) {
    Serial.print("Remote Type: ");
    switch (val.decode_type) {
        case NEC:
            Serial.print("NEC");
            break;
        case SONY:
            Serial.print("SONY");
            break;
        case RC5:
            Serial.print("RC5");
            break;
        case RC6:
            Serial.print("RC6");
            break;
        case DISH:
            Serial.print("DISH");
            break;
        case SHARP:
            Serial.print("SHARP");
            break;
        case JVC:
            Serial.print("JVC");
            break;
        case SANYO:
            Serial.print("SANYO");
            break;
        case MITSUBISHI:
            Serial.print("MITSUBISHI");
            break;
        case SAMSUNG:
            Serial.print("SAMSUNG");
            break;
        case LG:
            Serial.print("LG");
            break;
        case WHYNTER:
            Serial.print("WHYNTER");
            break;
        case AIWA_RC_T501:
            Serial.print("AIWA_RC_T501");
            break;
        case PANASONIC:
            Serial.print("PANASONIC");
            break;
        case DENON:
            Serial.print("DENON");
            break;
        default:
        case UNKNOWN:
            Serial.print("UNKNOWN");
            break;
    }
    Serial.print(" --- Value: ");
    Serial.println(results.value, HEX);
} // printRemoteValue()

/**
 * Simple loop code to echo IR values
 */
void remoteTypeLoop() {
    if (irrecv.decode(&results)) {
        printRemoteValue(results);
    }
    irrecv.resume();
}

/**
 * Loop code to test control via IR
 */
void loop_control() {
    if (irrecv.decode(&results)) {  // if IR value was read
        // print code in HEX
        Serial.println(results.value, HEX);
        switch (results.value) {
            case 0x2FD807F: // keypad "1"
                // turn on LED for 2 seconds
                digitalWrite(redPin, HIGH);
                delay(2000);
                digitalWrite(redPin, LOW);
                break;
            case 0x2FD40BF: // keypad "2"
                // toggle LED on or off
                if (toggleState == 0) {
                    digitalWrite(yellowPin, HIGH);
                    toggleState = 1;
                }
                else {
                    digitalWrite(yellowPin, LOW);
                    toggleState = 0;
                }
        }
        irrecv.resume();
    }  // if IR value was read
} // loop_control()

void loop() {
    remoteTypeLoop();
    delay(500L);
}