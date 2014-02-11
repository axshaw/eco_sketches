#include <ArduinoRobot.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Sensors
const int lightPin = TKD1; //LDR pin - input
OneWire oneWire(TKD2);
DallasTemperature sensors(&oneWire);

//RGB Status LED
const int redLEDPin = TKD5; 
const int greenLEDPin = TKD3;
const int blueLEDPin = TKD4;

String screenText = "";
int compassValue;



void setup()  {
  pinMode(greenLEDPin, OUTPUT)
  pinMode(blueLEDPin, OUTPUT)
  pinMode(redLEDPin, OUTPUT)
  //digitalWrite(ledPin, 0);             
  //fire up modules
  Serial.begin(9600);

  Robot.begin();
  Robot.beginTFT();
 // Robot.beginSD();
 // Robot.displayLogos();
     // Start up the library
  sensors.begin();
  
}

void setLEDstatusRed()  {
  Robot.digitalWrite(greenLEDPin,0);
  Robot.digitalWrite(blueLEDPin,0);
  Robot.digitalWrite(redLEDPin, 1);             
}

void setLEDstatusGreen()  {
  Robot.digitalWrite(greenLEDPin,1);
  Robot.digitalWrite(blueLEDPin,0);
  Robot.digitalWrite(redLEDPin, 0);             
}

void setLEDstatusBlue()  {
  Robot.digitalWrite(greenLEDPin,0);
  Robot.digitalWrite(blueLEDPin,1);
  Robot.digitalWrite(redLEDPin, 0);             
}

void writeToScreen(String text) {
  Robot.stroke(255, 255, 255);        // choose the color white
  Robot.text(screenText,0,0);
  screenText = text;
  Robot.stroke(0, 0, 0);              // choose the color black
  Robot.text(text,0,0);
}

void loop()  {
   sensors.requestTemperatures(); // Send the command to get temperatures
  compassValue = Robot.compassRead();
  writeToScreen("Facing - " + String(compassValue));
  //Robot.stroke(0, 0, 0);              // choose the color black
  //Robot.text("Facing - " + compassValue,0,0);
  Serial.println(Robot.analogRead(lightPin));
  Serial.println(sensors.getTempCByIndex(0) );
  Serial.println(compassValue);
  setLEDstatusGreen();
  delay(2000);                         // 4 second LED blink, good for wireless programming
  setLEDstatusRed();  
  delay(2000);
  setLEDstatusBlue();
  //Robot.fill(0,0,0);
  //Robot.stroke(255, 255, 255);        // choose the color white
  //Robot.text("Facing" + compassValue,0,0);
  delay(4000);  
}
