// include display the library code:
#include <TM1637Display.h>

/*
  RPM meter code ()
  * using a hall effect sensor KY-024
  * using a 7 segment TM1637 display
*/

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

//revlimit relay pins
#define RELAY1 6

// read the hall effect sensor
const int hallPin = 9;
const unsigned long sampleTime = 100;
const int maxRPM = 10200;

const uint8_t SEG_ITU[] = {
    SEG_F | SEG_E,                 // I
    SEG_D | SEG_E | SEG_F | SEG_G, // t
    SEG_C | SEG_D | SEG_E,         // u
    SEG_D                          //_
};
const uint8_t SEG_RACE[] = {
    SEG_G | SEG_E,                                 // r
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
    SEG_A | SEG_F | SEG_D | SEG_E,                 // C
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D          // E
};

void setup()
{
  pinMode(RELAY1, OUTPUT);
  display.setBrightness(0x0f);
  display.setSegments(SEG_ITU);
  delay(1000);
  display.setSegments(SEG_RACE);
  delay(1000);
  pinMode(hallPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int rpm = getRPM();
  display.showNumberDec(rpm, false);
  delay(100);
  /* 
    Velocity section is not necessary for me
    int velocity = getVelocity();
    display.showNumberDec(velocity, false);
 */
  //REVLIMITER Setted to 6500rpm
  revLimit();
 }

int getRPM()
{
  // sample for sampleTime in millisecs
  int kount = 0;
  boolean kflag = LOW;
  unsigned long currentTime = 0;
  unsigned long startTime = millis();
  while (currentTime <= sampleTime)
  {
    if (digitalRead(hallPin) == HIGH)
    {
      kflag = HIGH;
    }
    if (digitalRead(hallPin) == LOW && kflag == HIGH)
    {
      kount++;
      kflag = LOW;
    }
    currentTime = millis() - startTime;
  }
  int kount2rpm = int(6000. / float(sampleTime)) * kount;
  return kount2rpm;
}

int getVelocity()
// revolutions per minute to velocity by gear ratio 11 toot engine, 44 tooth axle, 11" diameter wheel to km/h
{
  int rpm = getRPM();
  int velocity = int(rpm * 0.01315974);
  return velocity;
}


boolean revLimit()
{
  int rpm = getRPM();
  if(rpm>6500){
    digitalWrite(RELAY1,HIGH);
  } else {
    digitalWrite(RELAY1,LOW);
  }
}
