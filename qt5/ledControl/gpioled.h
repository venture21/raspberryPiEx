#ifndef GPIOLED_H
#define GPIOLED_H

class GpioLED
{
public:
    //gpioLED();
    int ledBlink(int gpio);
    int ledCTRL(int gpioPin, int ledCmd);
};

#endif // GPIOLED_H
