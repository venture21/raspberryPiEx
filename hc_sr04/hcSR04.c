#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  //gettimeofday()
#include <unistd.h>	   //gettimeofday()

#define trig 4
#define echo 5

struct timeval UTCtime_r;

void disp_runtime(struct timeval UTCtime_s, struct timeval UTCtime_e)
{
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

}


int main(int argc, char *argv[])
{
	int startTime, endTime;
	float distance, before_distance, delta_s;
	double velocity;
	int flag = 0;
	int count=0;
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

		if (!flag && count>0)
		{	//trigger를 입력하는 시점에서 시간 저정
			gettimeofday(&UTCtime_t1, NULL); // UTC 현재 시간 구하기(마이크로초까지)
			disp_runtime(UTCtime_t2, UTCtime_t1);
		}
		else if(flag && count>0)
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

		if (count > 0)
			printf("runtime : %ld\n", UTCtime_r.tv_usec);

		// 거리 계산 공식
		before_distance = distance;
		distance = (endTime - startTime) / 58;
		printf("distance %.2f cm\n", distance);

		if (distance > before_distance)
			delta_s = distance - before_distance;
		else
			delta_s = before_distance - distance;

		velocity = (delta_s*10000)/ (UTCtime_r.tv_usec);  // unit : m/s
		printf("velocity %f m/s\n", velocity);
		count++;
	}
	return 0;
}

