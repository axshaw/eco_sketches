#include <ArduinoRobot.h>
#include <OneWire.h>

// Sensors
const int lightPin = TKD1;

//OneWire ds(TKD2);

// RGB Status LED
const int redLEDPin = TKD5;
const int greenLEDPin = TKD3;
const int blueLEDPin = TKD4;

void setup() {
  pinMode(redLEDPin,   OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin,  OUTPUT);
  
  Robot.begin();
  Robot.beginTFT();
}

void loop() {
  String x = "0.00";
  writeToScreen(stringToChar("Direction:   " + Robot.compassRead()), 2);
  writeToScreen(stringToChar("Temperature: " + x + " deg C"), 16);
  writeToScreen(stringToChar("Light:       " + Robot.analogRead(lightPin)), 30);
  setLEDStatus(true, false, false);
  setLEDStatus(false, true, false);
  setLEDStatus(false, false, true);
}

void writeToScreen(char* text, int y) {
  Robot.background(255, 255, 255);
  Robot.stroke(0, 0, 0);
  Robot.text(text,2,y);
}

char* stringToChar(String text) {
  char buffer[text.length()];
  text.toCharArray(buffer, text.length());
  return buffer;
}

void setLEDStatus(boolean red, boolean green, boolean blue)  {
  Robot.digitalWrite(greenLEDPin, red);
  Robot.digitalWrite(blueLEDPin,  green);
  Robot.digitalWrite(redLEDPin,   blue);
  delay(1000);
}
