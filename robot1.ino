#include <ArduinoRobot.h>
#include <OneWire.h>

// Pins
const int lightPin = TKD0;
const int temperatureSensorPin = TKD1;
const int redLEDPin = TKD5;
const int greenLEDPin = TKD3;
const int blueLEDPin = TKD4;

// Sensors
OneWire temperatureSensor1(temperatureSensorPin);

void setup() {
  pinMode(redLEDPin,   OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin,  OUTPUT);
  
  Robot.begin();
  Robot.beginTFT();
}

void loop() {  
  String dir = String(Robot.compassRead());
  String temp = getTemperature(temperatureSensor1);
  String light = String(Robot.analogRead(lightPin));
  Robot.background(0, 0, 0);
  writeToScreen("Dir   : " + dir, 2);
  writeToScreen("Temp  : " + temp  + " deg C", 16);
  writeToScreen("Light : " + light, 30);
  setLEDStatus(true, false, false);
  setLEDStatus(false, true, false);
  setLEDStatus(false, false, true);
}

void writeToScreen(String text, int y) {
  char buffer[20];
  text.toCharArray(buffer, 20);
  Robot.stroke(255, 255, 255);
  Robot.text(buffer,2,y);
}

void setLEDStatus(boolean red, boolean green, boolean blue)  {
  Robot.digitalWrite(greenLEDPin, red);
  Robot.digitalWrite(blueLEDPin,  green);
  Robot.digitalWrite(redLEDPin,   blue);
  delay(1000);
}

String getTemperature(OneWire ds) {
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  String temp = "";

  if ( !ds.search(addr)) {
      ds.reset_search();
      return "NMA";
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      return "CRC";
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  
  if (SignBit) {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;

  if (SignBit) {
     temp = "-";
  }
  
  temp += Whole;
  temp += ".";
  
  if (Fract < 10) {
     temp += "0";
  }
  
  temp += Fract;
  
  return temp;
}
