#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

//시간관련 헤더파일
#include <time.h>
#include <sys/time.h>

#define SW1		1 //wPin 1
#define SW2		3 //wPin 3
#define LED		2 //wPin 2

#define DEBUG

void pinAssign(void)
{
	pinMode(SW1, INPUT);
	pinMode(LED, OUTPUT);
	pinMode(SW2, INPUT);
}


int waitStart(struct timeval *start_tv)
{
	// SW1이 눌리면 START
	while (digitalRead(SW1));

	// START시간 저장
	if ((gettimeofday(start_tv, NULL)) == -1)
	{
		perror("gettimeofday() call error");
		return -1;
	}
}

int main()
{
	//time_t start_t, end_t;
	struct timeval start_tv, stop_tv, diff_tv;
	char val1, val2;
	int retValue;

	wiringPiSetup();

	pinAssign();

	if (waitStart(&start_tv) < 0)
		return -1;

#ifdef DEBUG
	printf("Start Time\n");
	printf("%ld.%ld\n", start_tv.tv_sec, start_tv.tv_usec);
#endif

	while (1)
	{
		usleep(500);

		if ((gettimeofday(&stop_tv, NULL)) == -1)
		{
			perror("gettimeofday() call error");
			return -1;
		}

		if (start_tv.tv_usec > stop_tv.tv_usec)
		{
			stop_tv.tv_sec--;
			diff_tv.tv_usec = 1000000 + stop_tv.tv_usec - start_tv.tv_usec;
			diff_tv.tv_sec = stop_tv.tv_sec - start_tv.tv_sec;
		}
		else
		{
			diff_tv.tv_usec = stop_tv.tv_usec - start_tv.tv_usec;
			diff_tv.tv_sec = stop_tv.tv_sec - start_tv.tv_sec;
		}

		//printf("DIFF Time");
		printf("Time:%ld.%ld\r", diff_tv.tv_sec, diff_tv.tv_usec);
		fflush(stdout);
	}
	/*


		val2 = digitalRead(SW2);
		if (val1 && val2)//check if the button is pressed, if yes, turn on the LED
			digitalWrite(2, LOW);
		else
			digitalWrite(2, HIGH);
	}*/
}
