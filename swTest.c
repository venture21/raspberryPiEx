#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define SW1 0  //GPIO17





int main(void)
{
	int swFlag=0;
	int count=0;
	// Init wiringPi
	wiringPiSetup();
	
	//pinAssign
	pinMode(SW1, INPUT);

	while(1)
	{
		//swFlag==0이고, 스위치가 눌렸을때
		/*
		if(digitalRead(SW1)&& !swFlag)
		{
			count++;
			printf("Switch ON:%d\n",count);
			swFlag=1;
		}
		else if (!digitalRead(SW1) && swFlag)
		{
			swFlag=0;
		}	
		usleep(10000);
		*/
		if(digitalRead(SW1))
		{
			printf("Switch ON:%d\n",count);
			count++;
		}
		usleep(10000);
	}

}
