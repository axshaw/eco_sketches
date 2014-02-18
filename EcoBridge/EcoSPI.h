#ifndef EcoSPI_h
#define EcoSPI_h

#include "Arduino.h"
#include "EcoSensors.h"
#include <SPI.h>

#define START_BYTE       0x00
#define NEXT_BYTE        0xFF

#define RTC              0x01
#define ULTRASONIC_0     0x02
#define ULTRASONIC_45    0x03
#define ULTRASONIC_90    0x04
#define ULTRASONIC_135   0x05
#define ULTRASONIC_180   0x06
#define ULTRASONIC_225   0x07
#define ULTRASONIC_270   0x08
#define ULTRASONIC_315   0x09
#define TEMPERATURE_LEFT 0x10
#define TEMPERTURE_RIGHT 0x11
#define LIGHT_LEFT       0x12
#define LIGHT_RIGHT      0x13
#define COMPASS_BEARING  0x14
#define CO_PPM           0x15

#define SPI_HANDSHAKE      0xF0
#define DEVICE_ULTRASONIC  0xF1

class EcoSPI
{
  public:
    EcoSPI(int interruptPin, byte device);
    void getReadings(EcoSensors &es);
  private:
    EcoSensors _es;
    boolean _handshake();
    void _getReading(byte sensor, byte &a);
    void _getReading(byte sensor, int &a);
    void _getReading(byte sensor, float &a);
    int _interruptPin;
    byte _device;
    void _send(byte command, int value);
    void _listen();
    char _spi_transfer(volatile char data);
    byte _transferAndWait (const byte what);
    long _bytesToInteger(byte b[4]);
    void _integerToBytes(long val, byte b[4]);
};

#endif
