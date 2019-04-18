#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int trig = 23;
	int echo = 24;
	int startTime, endTime;
	float distance;

	// wiringPiSetup이 실패할 경우 종료 
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	while (1) {
		// Trig신호 생성 (10us)
		digitalWrite(trig, LOW);
		delay(500);
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
[출처] 초음파 모듈 제어(HC - SR04) - C언어 | 작성자 호기심

