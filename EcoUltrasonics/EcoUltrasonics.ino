// Written by Nick Gammon
// April 2011

#include <NewPing.h>
#include "pins_arduino.h"

#define SONAR_NUM        8    // Number or sensors.
#define MAX_DISTANCE     500  // Maximum distance (in cm) to ping.
#define PING_INTERVAL    33   // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

#define SPI_HANDSHAKE    0xF0
#define SPI_ULTRASONICS  0xF1

#define ULTRASONIC_0     0x02
#define ULTRASONIC_45    0x03
#define ULTRASONIC_90    0x04
#define ULTRASONIC_135   0x05
#define ULTRASONIC_180   0x06
#define ULTRASONIC_225   0x07
#define ULTRASONIC_270   0x08
#define ULTRASONIC_315   0x09

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {        // Sensor object array.
  NewPing(3,  3,  MAX_DISTANCE),    // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(4,  4,  MAX_DISTANCE),
  NewPing(5,  5,  MAX_DISTANCE),
  NewPing(6,  6,  MAX_DISTANCE),
  NewPing(7,  7,  MAX_DISTANCE),
  NewPing(8,  8,  MAX_DISTANCE),
  NewPing(9,  9,  MAX_DISTANCE),
  NewPing(A0,  A0,  MAX_DISTANCE)
};

// what to do with incoming data
volatile byte command = 0;
volatile int byteCount = 0;

// start of transaction, no command yet
void ss_falling ()
{
  command = 0;
  byteCount = 0;
}  // end of interrupt service routine (ISR) ss_falling

void setup (void)
{
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  // interrupt for SS falling edge
  attachInterrupt (0, ss_falling, FALLING);
  
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}  // end of setup

void loop (void)
{
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}  // end of loop

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;
 
  switch (command)
  {
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    break;
  case SPI_HANDSHAKE:
    SPDR = SPI_ULTRASONICS;
    break;
  case ULTRASONIC_0:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[0] & 0xff);
    } else {
      SPDR = (byte)((cm[0] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_45:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[1] & 0xff);
    } else {
      SPDR = (byte)((cm[1] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_90:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[2] & 0xff);
    } else {
      SPDR = (byte)((cm[2] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_135:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[3] & 0xff);
    } else {
      SPDR = (byte)((cm[3] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_180:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[4] & 0xff);
    } else {
      SPDR = (byte)((cm[4] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_225:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[5] & 0xff);
    } else {
      SPDR = (byte)((cm[5] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_270:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[6] & 0xff);
    } else {
      SPDR = (byte)((cm[6] >> 8) & 0xff);
    }
    break;
  case ULTRASONIC_315:
    if (byteCount++ == 0) {
      SPDR = (byte)(cm[7] & 0xff);
    } else {
      SPDR = (byte)((cm[7] >> 8) & 0xff);
    }
    break;
  } // end of switch
}  // end of interrupt service routine (ISR) SPI_STC_vect

