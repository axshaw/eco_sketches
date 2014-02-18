#ifndef EcoSensors_h
#define EcoSensors_h

#include "Arduino.h"

struct EcoSensors
{
  float rtc;
  int ultrasonic0;
  int ultrasonic45;
  int ultrasonic90;
  int ultrasonic135;
  int ultrasonic180;
  int ultrasonic225;
  int ultrasonic270;
  int ultrasonic315;
  float temperatureLeft;
  float temperatureRight;
  int lightLeft;
  int lightRight;
  int compassBearing;
  int COppm;
};

#endif
