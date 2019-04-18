#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int trig = 23;
	int echo = 24;
	int startTime, endTime;
	float distance;

	// wiringPiSetup�� ������ ��� ���� 
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	while (1) {
		// Trig��ȣ ���� (10us)
		digitalWrite(trig, LOW);
		delay(500);
		digitalWrite(trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig, LOW);

		// Echo back�� '1'�̸� ���۽ð� ��� 
		while (digitalRead(echo) == 0);
		startTime = micros();
		// Echo back�� '1'�̸� ��� 
		while (digitalRead(echo) == 1);
		// Echo back�� '0'�̸� ����ð� ��� 
		endTime = micros();
		// �Ÿ� ��� ����
		distance = (endTime - startTime) / 58;
		printf("distance %.2f cm\n", distance);
	}
	return 0;
}
[��ó] ������ ��� ����(HC - SR04) - C��� | �ۼ��� ȣ���

