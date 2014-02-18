// Written by Nick Gammon
// April 2011

#include "EcoSPI.h"
#include "EcoSensors.h"
#include <SPI.h>
#include "pins_arduino.h"

EcoSPI utrasonic_espi(5, DEVICE_ULTRASONIC);

void setup (void)
{
  Serial.begin (115200);
  Serial.println ();
  
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  
}  // end of setup

void loop (void)
{
  EcoSensors es;
  
  utrasonic_espi.getReadings(es);
  
  Serial.println(es.ultrasonic0);
  Serial.println(es.ultrasonic45);
  Serial.println(es.ultrasonic90);
  Serial.println(es.ultrasonic135);
  Serial.println(es.ultrasonic180);
  Serial.println(es.ultrasonic225);
  Serial.println(es.ultrasonic270);
  Serial.println(es.ultrasonic315);
  
  delay (1000);  // 1 second delay 
}  // end of loop
