


// RGB Status LED
const int echoPin = 9;
const int echoPin2 = 6;
const int trigPin = 7;

void firePing()  {
    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);// attach pin 3 to Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
}

void setup() {
 
  Serial.begin(9600);
}

void loop()
{

  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, duration2, cm, cm2;


firePing();
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode (echoPin, INPUT);//attach pin 4 to Echo
  duration = pulseIn(echoPin, HIGH);
 firePing(); 
  
  pinMode (echoPin2, INPUT);//attach pin 4 to Echo
  duration2 = pulseIn(echoPin2, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  cm2 = microsecondsToCentimeters(duration2);
  
  Serial.print(cm);
  Serial.print(" : ");
  Serial.print(cm2);
  Serial.println();
 
  delay(100);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
