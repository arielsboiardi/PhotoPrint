#include <Arduino.h>

// Constants 
#define BAUDRATE 9600

#define LED_ON    HIGH
#define LED_OFF   LOW

#define LED_0 2
#define LED_1 3
#define LED_2 4
#define LED_3 5

// Commands

const char LED_0_ON_CMD [] = "0 on";  // LED_0_ON_CMD string.
const char LED_1_ON_CMD [] = "1 on";  // LED_1_ON_CMD string.
const char LED_2_ON_CMD [] = "2 on";  // LED_2_ON_CMD string.
const char LED_3_ON_CMD [] = "3 on";  // LED_3_ON_CMD string.

const char LED_0_OFF_CMD [] = "0 off"; // LED_0_OFF_CMD string.
const char LED_1_OFF_CMD [] = "1 off"; // LED_1_OFF_CMD string.
const char LED_2_OFF_CMD [] = "2 off"; // LED_2_OFF_CMD string.
const char LED_3_OFF_CMD [] = "3 off"; // LED_3_OFF_CMD string.

// Global valuables
bool led_0_state = LED_OFF;
bool led_1_state = LED_OFF;
bool led_2_state = LED_OFF;
bool led_3_state = LED_OFF;


void serialEvent(){

  // Read the command from the serial port
  String command = Serial.readString();
  command.trim();

  // Check the command
  if(command == LED_0_ON_CMD){
    led_0_state = LED_ON;
  } else if(command == LED_0_OFF_CMD){
    led_0_state = LED_OFF;
  } else if(command == LED_1_ON_CMD){
    led_1_state = LED_ON;
  } else if(command == LED_1_OFF_CMD){
    led_1_state = LED_OFF;
  } else if(command == LED_2_ON_CMD){
    led_2_state = LED_ON;
  } else if(command == LED_2_OFF_CMD){
    led_2_state = LED_OFF;
  } else if(command == LED_3_ON_CMD){
    led_3_state = LED_ON;
  } else if(command == LED_3_OFF_CMD){
    led_3_state = LED_OFF;
  }

}


void setup() {
  // Set up the serial communication
  Serial.begin(BAUDRATE);

  // Set up the LED pins
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  // Set the initial state of the LEDs
  digitalWrite(LED_0, led_0_state);
}

void loop() {

  digitalWrite(LED_0, led_0_state);
  digitalWrite(LED_1, led_1_state);
  digitalWrite(LED_2, led_2_state);
  digitalWrite(LED_3, led_3_state);

}