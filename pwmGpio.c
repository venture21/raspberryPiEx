#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define loopCount	10	
#define delayMax	1024

// ./pwmGpio [PinNumer]
int main(int argc, char** argv)
{
	int gpioNo;
	int i;
	int delayTime;

	// STEP 1. WiringPi Init
	wiringPiSetup();
	
	// Pin number Error
	if(argc<2)
	{
		printf("Usage : %s gpioNo\n", argv[0]);
		return -1;
	}
	
	gpioNo = atoi(argv[1]);
	
	// STEP 2. Pin direction setup
	pinMode(gpioNo, OUTPUT);
	
	for(i=0;i<loopCount;i++)
	{
		for(delayTime=0;delayTime<delayMax;delayTime++)
		{
			// STEP 3. Pin Write
			digitalWrite(gpioNo, HIGH);
			usleep(delayTime);
			digitalWrite(gpioNo, LOW);
			usleep(delayMax-delayTime);
		}	
	}
	
	return 0;
}
