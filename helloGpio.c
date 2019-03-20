#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>

#define delayTime	500	//500ms
#define loopCount	10	

// ./helloGpio [PinNumer]
int main(int argc, char** argv)
{
	int gpioNo;
	int i;

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
		// STEP 3. Pin Write
		digitalWrite(gpioNo, HIGH);
		delay(delayTime);
		digitalWrite(gpioNo, LOW);
		delay(delayTime);	
	}
	
	return 0;
}
