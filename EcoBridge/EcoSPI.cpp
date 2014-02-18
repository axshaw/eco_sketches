#include "Arduino.h"
#include <SPI.h>
#include "EcoSPI.h"

EcoSPI::EcoSPI(int interruptPin, byte device)
{
  _interruptPin = interruptPin;
  _device = device;
  
  pinMode(_interruptPin, OUTPUT);
  digitalWrite(_interruptPin, HIGH);
}

void EcoSPI::getReadings(EcoSensors &es)
{
  _handshake();
  
  if (_device == 0) {
    _getReading(RTC, es.rtc);
  }
  if (_device == DEVICE_ULTRASONIC) {
    _getReading(ULTRASONIC_0,     es.ultrasonic0);
    _getReading(ULTRASONIC_45,    es.ultrasonic45);
    _getReading(ULTRASONIC_90,    es.ultrasonic90);
    _getReading(ULTRASONIC_135,   es.ultrasonic135);
    _getReading(ULTRASONIC_180,   es.ultrasonic180);
    _getReading(ULTRASONIC_225,   es.ultrasonic225);
    _getReading(ULTRASONIC_270,   es.ultrasonic270);
    _getReading(ULTRASONIC_315,   es.ultrasonic315);
  }
  if (_device == 0) {
    _getReading(TEMPERATURE_LEFT, es.temperatureLeft);
    _getReading(TEMPERTURE_RIGHT, es.temperatureRight);
    _getReading(LIGHT_LEFT,       es.lightLeft);
    _getReading(LIGHT_RIGHT,      es.lightRight);
    _getReading(COMPASS_BEARING,  es.compassBearing);
    _getReading(CO_PPM,           es.COppm);
  }
}

boolean EcoSPI::_handshake() {
  byte a;

  // enable Slave Select
  digitalWrite(_interruptPin, LOW);   
  
  _transferAndWait (SPI_HANDSHAKE);  // handshake command
  _transferAndWait (0);
  a = _transferAndWait (0);
  
  // disable Slave Select
  digitalWrite(_interruptPin, HIGH);
  
  Serial.println ("Handshake:");
  return (a == _device);
}

void EcoSPI::_getReading(byte sensor, byte &a) {
  // enable Slave Select
  digitalWrite(_interruptPin, LOW);   
  
  _transferAndWait (sensor);  // handshake command
  _transferAndWait (0);
  a = _transferAndWait (0);
  
  // disable Slave Select
  digitalWrite(_interruptPin, HIGH);
}

void EcoSPI::_getReading(byte sensor, int &a) {
  // enable Slave Select
  byte bytea, byteb;
  
  digitalWrite(_interruptPin, LOW);   
  
  _transferAndWait (sensor);  // handshake command
  _transferAndWait (0);
  bytea = _transferAndWait (0);
  byteb = _transferAndWait (0);
  
  a = 0;
  a |= byteb << 8;
  a |= bytea;
  
  // disable Slave Select
  digitalWrite(_interruptPin, HIGH);
}

void EcoSPI::_getReading(byte sensor, float &a) {
  // enable Slave Select
  digitalWrite(_interruptPin, LOW);   
  
  _transferAndWait (sensor);  // handshake command
  _transferAndWait (0);
  a = _transferAndWait (0);
  
  // disable Slave Select
  digitalWrite(_interruptPin, HIGH);
}


void EcoSPI::_send(byte command, int value)
{
  digitalWrite(_interruptPin, LOW);
  
  //SPI.transfer(START_BYTE);
  
  //SPI.transfer(command);
  
  //SPI.transfer(value);
  
  //for (int i = 0; i < sizeof(command); i++) {
  //  SPI.transfer(command[i]);
  //}

  //SPI.transfer(END_BYTE);
  
  digitalWrite(_interruptPin, HIGH); 
}

void EcoSPI::_listen()
{
  
}

char EcoSPI::_spi_transfer(volatile char data)
{
 SPDR = data; // Start the transmission
 //while (!(SPSR & (1<
 //{
 //};
 return SPDR; // return the received byte
}



byte EcoSPI::_transferAndWait (const byte what)
{
  byte a = SPI.transfer (what);
  delayMicroseconds (20);  // everything is already held so no need for big delay
  return a;
} // end of transferAndWait

long EcoSPI::_bytesToInteger(byte b[4]) {
  long val = 0;
  val = b[0] << 24;
  val |= b[1] << 16;
  val |= b[2] << 8;
  val |= b[3];
  return val;
}

void EcoSPI::_integerToBytes(long val, byte b[4]) {
  b[0] = (byte )((val >> 24) & 0xff);
  b[1] = (byte )((val >> 16) & 0xff);
  b[2] = (byte )((val >> 8) & 0xff);
  b[3] = (byte )(val & 0xff);
}
