#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

//시간관련 헤더파일
#include <time.h>
#include <sys/time.h>

#define SW1		1 //wPin 1
#define SW2		3 //wPin 3
#define LED		2 //wPin 2

//#define DEBUG

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
	return 0;
}

int updateTime(struct timeval *start_tv, struct timeval *lap_tv, struct timeval *diff_tv)
{
		if ((gettimeofday(lap_tv, NULL)) == -1)
		{
			perror("gettimeofday() call error");
			return -1;
		}

		if (start_tv->tv_usec > lap_tv->tv_usec)
		{
			lap_tv->tv_sec--;
			diff_tv->tv_usec = 1000000 + lap_tv->tv_usec - start_tv->tv_usec;
			diff_tv->tv_sec = lap_tv->tv_sec - start_tv->tv_sec;
		}
		else
		{
			diff_tv->tv_usec = lap_tv->tv_usec - start_tv->tv_usec;
			diff_tv->tv_sec = lap_tv->tv_sec - start_tv->tv_sec;
		}
		return 0;
}

void displayTime(struct timeval *diff_tv)
{
	int sec, msec;
	sec = diff_tv->tv_sec % 100;
	msec = diff_tv->tv_usec / 10000;
	printf("LapTime:%2d.%d\r", sec, msec);
	fflush(stdout);
}

int main()
{
	struct timeval start_tv, lap_tv, diff_tv;
	char val1, val2;
	int sw1_flag = 0;
	
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
		//5ms(5000us)마다 중간시간 업데이트
		usleep(10000);
		if (updateTime(&start_tv, &lap_tv, &diff_tv) < 0)
			return -1;
		
		displayTime(&diff_tv);

		if (!digitalRead(SW1) && !sw1_flag)
		{
			sw1_flag = 1;
			printf("\n");
		}
		else if(digitalRead(SW1) && sw1_flag)
		{
			sw1_flag = 0;
		}

		if (!digitalRead(SW2))
			break;
	}
}
