
/**
 * @file main.cpp
 * @brief This file contains the implementation of the PhotoPrintController for Arduino Nano ESP32.
 * 
 * The program controls a stage and a projector using serial and I2C communication respectively.
 * It provides a command-line interface (CLI) for controlling the stage and projector.
 * 
 * The program reads commands from the serial interface and processes them using the SimpleCLI library.
 * 
 * @note The stage controller communicates via (hardware) Serial1 interface on pins D6 (TX) and D7 (RX), 
 * the logical level of the stage controller is the standard RS-232 level (±12V), a MAX2323 is used to convert the levels.
 * The stage controller is an Olympus BX3SSU, the communication protocol is ASCII-based, and the commands are terminated "\r\n".
 * A null-modem cable is used to connect the MAX2323 to the stage controller.
 * The projector is a custom device with a blue LED based on the DLP350 chipset from Texas Instruments. The device is connected 
 * to the ESP32 via I2C on pins A4 (SCL) and A5 (SDA) and to the J17 connector on the stage controller. 
 * The device has an I2C address of 0x1A.
 * 
 * @author Ariel S. Boiardi
 * @date 15/01/2025
 */
#include <Arduino.h>    // Include the Arduino library
#include <Wire.h>       // Include the Wire library for I2C communication
#include <SimpleCLI.h>  // Include the SimpleCLI library for CLI parsing

// Constants
#define I2C_ADDRESS 0x1A  // I2C address of the projector on connector J17

// CLI 
SimpleCLI cli;
Command stage;
Command projector;
Command print;

// Stage 
int stageSpeedDefault = 5;
bool StageLog = false;

/**
 * @brief Reads a response from the stage on Serial 1 until a newline character is encountered.
 * 
 * This function waits until data is available on the Serial1 interface, then reads the incoming
 * data as a string until a newline character ('\n') is encountered. The newline character is 
 * removed from the end of the string before returning the response.
 * 
 * @return String The response read from the Serial1 interface, with the newline character removed.
 */
String readSerialResponse() {
  
  while (!Serial1.available()) {
    // Busy-wait until data is available
  }
  String response = Serial1.readStringUntil('\n');
  response.remove(response.length() - 1);
  return response;
}

/**
 * @brief Initializes the stage by sending a sequence of startup commands.
 * 
 * This function sends a predefined sequence of commands to the stage controller
 * via the Serial1 interface to initialize the stage. The response from the stage 
 * controller is read and optionally printed to the Serial monitor.
 */
void StageStart(){

  String response;
  
  String startupSequence[] = {
		"3LOG IN",
		"3UNIT?",
		"3XYINIT?",
		"3UNIT?",
		"3XYINIT",
		"3UNIT?",
		"3XYP?",
		"3JGXY 1",
		"3JGXDR 1",
		"3JGYDR 1",
    "3XYSPD " + String(stageSpeedDefault),
  };

  for (const String command : startupSequence) {
    Serial.println(command); // Optionally print the command to the Serial monitor
    Serial1.print(String(command) + "\r\n");
    response = readSerialResponse();
    Serial.println(response); // Optionally print the response to the Serial monitor
  }
}

/**
 * @brief Executes a sequence of commands to close the stage.
 *
 * This function sends a predefined sequence of commands to the Serial1 interface
 * to close the stage.
 */
void StageClose(){
  String response;
  
  String closeSequence[] = {
    "3XYSTP",
    "3LOG OUT"
  };

  for (const String command : closeSequence) {
    Serial.println(command); // Optionally print the command to the Serial monitor
    Serial1.print(String(command) + "\r\n");
    response = readSerialResponse();
    Serial.println(response); // Optionally print the response to the Serial monitor
  }
}

/**
 * @brief Sets the speed of the stage to the specified value.
 * 
 * This function sends a command to the stage controller to set the speed of the stage
 * to the specified value. The speed is provided as an integer value representing the speed
 * in 10 um/s. The default speed value is set to 5, which corresponds to 50 um/s.
 * 
 * @param speed The speed value to set for the stage.
 */
void StageSpeed(int speed) {
  String response;
  Serial1.print("3XYSPD " + String(speed) + "\r\n");
  response = readSerialResponse();
  Serial.println(response);
}

/**
 * @brief Moves the stage to the specified position.
 * 
 * This function sends a command to the stage controller to move the stage to the specified position.
 * The position is provided as a string in the format "x,y" where x and y are the coordinates of the
 * desired offset from the current position in nanometers.
 * 
 * @param position A string representing the position to move the stage to in the format "x,y".
 */
void StageMove(String position) {
  String response;
  Serial1.print("3XYM " + position + "\r\n");
  response = readSerialResponse();
  Serial.println(response);
}

/**
 * @brief Callback function to handle stage commands.
 * 
 * This function processes commands related to the stage, such as starting,
 * closing, setting speed, and moving the stage to a specified position.
 * 
 * @param c Pointer to the command structure containing the command arguments.
 * 
 * The function checks for the following arguments:
 * - "start": If set, the stage is started.
 * - "close": If set, the stage is closed.
 * - "speed": If provided, the stage speed is set to the specified value.
 * - "move": If provided, the stage is moved to the specified position.
 * 
 * The function also prints relevant information to the serial monitor for debugging purposes.
 */
void stageCallback(cmd* c) {
  Command cmd(c);

  Argument start = cmd.getArg("start");
  Argument close = cmd.getArg("close");
  Argument speed = cmd.getArg("speed");
  Argument move = cmd.getArg("move");
  Argument log = cmd.getArg("log");
  Argument stop = cmd.getArg("stop");
  
  bool startFlag = start.isSet();
  bool closeFlag = close.isSet();
  int speedValue = speed.getValue().toInt();
  String moveValue = move.getValue();
  bool logFlag = log.isSet();
  bool stopFlag = stop.isSet();

  // Start or close the stage
  if (startFlag) {
    Serial.println("Starting stage");
    StageStart();
    return;
  } else if (closeFlag) {
    Serial.println("Closing stage");
    StageClose();
    return;
  }

  // Set the speed of the stage
  if (speedValue != 0) {
    Serial.println("Setting speed to " + String(speedValue));
        StageSpeed(speedValue);
  }

  // Move the by the specified offset
  if (moveValue != "0,0") {
    Serial.println("Moving stage to " + moveValue);
    StageMove(moveValue);
  }

  // Toggle the stage polling its position
  if (logFlag) {
    StageLog = !StageLog;
    if (StageLog) {
      Serial1.print("3NXYP 1\r\n");
    } else {
      Serial1.print("3NXYP 0\r\n");
    }
  }

  // Stop the stage
  if (stopFlag) {
    Serial.println("Stopping stage");
    Serial1.print("3XYSTP\r\n");
  }

}

// Projector

/**
 * @brief Toggles the projector's blue LED status.
 * 
 * This function sends an I2C command to toggle the blue LED of the projector.
 * It writes to the I2C device at the specified address and sets the appropriate
 * register to turn the blue LED on or off based on the provided status.
 * 
 * @param status A boolean value indicating the desired state of the blue LED.
 *               - true: Turn on the blue LED.
 *               - false: Turn off the blue LED.
 */
void projectorToggle(bool status) {
  Wire.beginTransmission(I2C_ADDRESS); 
  Wire.write(0x90); // register 0x10 with bit 7 set to 1 for write mode
  if (status) {
    Wire.write(0x04); // turn on blue led
  }
  else {
    Wire.write(0x00); // turn off blue led
  }
  Wire.endTransmission(); // end writing without sending stop bit
}

/**
 * @brief Callback function to handle projector commands.
 *
 * This function processes the given command to either turn on or turn off the projector.
 * It checks for the presence of "on" and "off" arguments in the command and toggles the
 * projector state accordingly.
 *
 * @param c Pointer to the command structure containing the arguments.
 */
void projectorCallback(cmd* c) {
  Command cmd(c);

  Argument on = cmd.getArg("on");
  Argument off = cmd.getArg("off");

  bool onFlag = on.isSet();
  bool offFlag = off.isSet();

  if (onFlag) {
    Serial.println("Turning on projector");
    projectorToggle(true);
    return;
  } else if (offFlag) {
    Serial.println("Turning off projector");
    projectorToggle(false);
    return;
  }

}

// Print

/**
 * @brief Prints from a specified length with initial and final wait.
 * 
 * This function sends a command to the stage controller to move the stage by the specified length
 * to photopolymerize the hydrogel. The length is provided as an integer value representing the length
 * in nanometers. The function also optionally waits a specified time before and after the print.
 * 
 * @param length The length of paper to print in micrometers.
 * @param wait An integer value representing the time to wait in seconds before and after the print.
 */
void printCallback(cmd* c) {
  Command cmd(c);

  Argument length = cmd.getArg("length");
  Argument wait = cmd.getArg("wait");

  int lengthValue = length.getValue().toInt();
  int waitValue = wait.getValue().toInt();

  Serial.println("Printing " + String(lengthValue/1000) + " um");
  projectorToggle(true);
  millisWait(waitValue * 1000);
  StageMove(String(lengthValue)+",0", true);
  millisWait(waitValue * 1000);
  projectorToggle(false);
  Serial.println("Print finished");

}


// PC Serial Communication

/**
 * @brief Reads a command from the serial interface and parses it.
 * 
 * This function reads a command from the serial interface and parses it using the SimpleCLI library.
 * The command is then processed by the appropriate callback function based on the command type.
 */
void serialEvent() {
  String command = Serial.readStringUntil('\n');
  command.trim();

  cli.parse(command);
}

/**
 * @brief Initializes the serial communication and I2C communication for the stage and projector. 
 * 
 * This function initializes the serial communication with the PC, the serial communication with the stage controller,
 * and the I2C communication with the projector. It also sets up the CLI commands for the stage and projector control.
 */
void setup() {

  // Set up the serial communication with PC
  Serial.begin(9600);

  // Set up serial communication with Olympus BX3SSU
  int rxPin = D7;
  int txPin = D6;
  Serial1.begin(19200, SERIAL_8E2, rxPin, txPin);

  // Set up I2C communication
  Wire.begin();

  // Set up stage CLI
  stage = cli.addCmd("stage", stageCallback);
  stage.addFlagArg("start");
  stage.addFlagArg("close");
  stage.addArg("speed", "0");
  stage.addArg("move", "0,0");
  stage.addFlagArg("log");
  stage.addFlagArg("stop");

  // Set up projector CLI
  projectorToggle(false);
  projector = cli.addCmd("projector", projectorCallback);
  projector.addFlagArg("on");
  projector.addFlagArg("off");

  // Set up print CLI
  print = cli.addCmd("print", printCallback);
  print.addPosArg("length");
  print.addArg("wait","0");

}

/**
 * @brief Main loop of the program.
 * 
 * This function is the main loop of the program that continuously reads 
 * incoming data from the stage controller on Serial1 interface.
 */
void loop() {
  String response = Serial1.readStringUntil('\n');
  response.remove(response.length() - 1);
  if (response.length() > 0){
    Serial.println(response);
  }

  delay(100);
}

