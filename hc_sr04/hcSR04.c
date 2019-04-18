#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  //gettimeofday()
#include <unistd.h>	   //gettimeofday()

#define trig 4
#define echo 5

void disp_runtime(struct timeval UTCtime_s, struct timeval UTCtime_e)
{
	struct timeval UTCtime_r;
	if ((UTCtime_e.tv_usec - UTCtime_s.tv_usec) < 0)
	{
		UTCtime_r.tv_sec = UTCtime_e.tv_sec - UTCtime_s.tv_sec - 1;
		UTCtime_r.tv_usec = 1000000 - UTCtime_e.tv_usec - UTCtime_s.tv_usec;
	}
	else
	{
		UTCtime_r.tv_sec = UTCtime_e.tv_sec - UTCtime_s.tv_sec;
		UTCtime_r.tv_usec = UTCtime_e.tv_usec - UTCtime_s.tv_usec;
	}
	printf("runtime : %ld sec %ld\n", UTCtime_r.tv_sec, UTCtime_r.tv_usec);
}


int main(int argc, char *argv[])
{
	int startTime, endTime;
	float distance;
	int flag = 0;
	struct timeval UTCtime_t1, UTCtime_t2;

	// wiringPiSetup이 실패할 경우 종료 
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	while (1) {
		// Trig신호 생성 (10us)
		digitalWrite(trig, LOW);
		delay(500);

		if (!flag)
		{	//trigger를 입력하는 시점에서 시간 저정
			gettimeofday(&UTCtime_t1, NULL); // UTC 현재 시간 구하기(마이크로초까지)
		}
		else
		{
			gettimeofday(&UTCtime_t2, NULL); // UTC 현재 시간 구하기(마이크로초까지)
			disp_runtime(UTCtime_t1, UTCtime_t2);
		}
		digitalWrite(trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig, LOW);

		// Echo back이 '1'이면 시작시간 기록 
		while (digitalRead(echo) == 0);
		startTime = micros();
		// Echo back이 '1'이면 대기 
		while (digitalRead(echo) == 1);
		// Echo back이 '0'이면 종료시간 기록 
		endTime = micros();
		// 거리 계산 공식
		distance = (endTime - startTime) / 58;
		printf("distance %.2f cm\n", distance);
	}
	return 0;
}

