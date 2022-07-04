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

// read the hall effect sensor
const int hallPin = 9;
const unsigned long sampleTime = 1000;
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
  delay(200);
  int velocity1 = getVelocity();
  display.showNumberDec(velocity1, false);
  delay(100);
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
  int kount2rpm = int(60000. / float(sampleTime)) * kount;
  return kount2rpm;
}

int getVelocity()
// revolutions per minute to velocity by gear ratio 11 toot engine, 40 tooth axle, 11" diameter wheel to km/h
{
  int rpm = getRPM();
  int velocity = int(rpm * 0.01447);
  return velocity;
}
