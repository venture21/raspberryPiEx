#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define relayDat	1

int main(int argc, char *argv[])
{

	// wiringPiSetup이 실패할 경우 종료 
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(relayDat, OUTPUT);

	while (1)
	{
		if (!flag)
		{
			flag = 1;
			digitalWrite(relayDat, LOW);
		}
		else
		{
			flag = 0;
			digitalWrite(relayDat, HIGH);
		}
		sleep(1);
	}

	return 0;
}