#include <Arduino.h>
#include <IRremote.h>

// define sensor pin
const int RECV_PIN = 4;

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
}

void loop() {
    if (irrecv.decode(&results)) {
        // print code in HEX
        Serial.println(results.value, HEX);
        /*
        switch (results.decode_type) {
            case NEC:
                Serial.println("NEC");
                break;
            case SONY:
                Serial.println("SONY");
                break;
            case RC5:
                Serial.println("RC5");
                break;
            case RC6:
                Serial.println("RC6");
                break;
            case DISH:
                Serial.println("DISH");
                break;
            case SHARP:
                Serial.println("SHARP");
                break;
            case JVC:
                Serial.println("JVC");
                break;
            case SANYO:
                Serial.println("SANYO");
                break;
            case MITSUBISHI:
                Serial.println("MITSUBISHI");
                break;
            case SAMSUNG:
                Serial.println("SAMSUNG");
                break;
            case LG:
                Serial.println("LG");
                break;
            case WHYNTER:
                Serial.println("WHYNTER");
                break;
            case AIWA_RC_T501:
                Serial.println("AIWA_RC_T501");
                break;
            case PANASONIC:
                Serial.println("PANASONIC");
                break;
            case DENON:
                Serial.println("DENON");
                break;
            default:
            case UNKNOWN:
                Serial.println("UNKNOWN");
                break;
        }
        */
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
    }
}