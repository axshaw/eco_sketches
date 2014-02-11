#include <ArduinoRobot.h>
#include <OneWire.h>

// Sensors
const int lightPin = TKD1;
OneWire temperatureSensor1(TKD2);

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
  writeToScreen("Dir   : " + String(Robot.compassRead()), 2);
  writeToScreen("Temp  : " + getTemperature(temperatureSensor1) + " deg C", 16);
  writeToScreen("Light : " + String(Robot.analogRead(lightPin)), 30);
  setLEDStatus(true, false, false);
  setLEDStatus(false, true, false);
  setLEDStatus(false, false, true);
}

void writeToScreen(String text, int y) {
  char buffer[20];
  text.toCharArray(buffer, 20);
  
  Robot.background(255, 255, 255);
  Robot.stroke(0, 0, 0);
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
