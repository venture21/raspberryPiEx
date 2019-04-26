#include "gpioled.h"
#include "wiringPi.h"

#define loopCount   10
#define delayTime   250

//GpioLED::gpioLED()
//{

//}


int GpioLED::ledBlink(int gpio)
{
   int  i;

   // Set GPIO as OUTPUT
   pinMode(gpio, OUTPUT);

   for (i = 0; i < loopCount; i++) {
      digitalWrite(gpio, HIGH);    // HIGH(1) 값을 출력 : LED ON
      delay(delayTime);
      digitalWrite(gpio,  LOW);    // LOW (0) 값을 출력 : LED OFF
      delay(delayTime);
   };

   return 0;
}


int GpioLED::ledCTRL(int gpioPin, int ledCmd)
{
    pinMode(gpioPin, OUTPUT);
    digitalWrite(gpioPin, ledCmd);

    return 0;
}
