#include "mainwindow.h"
#include <QApplication>
#include <wiringPi.h>
#include "gpioled.h"


#define gpioNo      1


int main(int argc, char *argv[])
{
    wiringPiSetup();

    GpioLED GpioLED1;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    GpioLED1.ledBlink(gpioNo);

    return a.exec();
}


