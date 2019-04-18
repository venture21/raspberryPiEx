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

	// wiringPiSetup�� ������ ��� ���� 
	if (wiringPiSetup() == -1)
		exit(1);

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	while (1) {
		// Trig��ȣ ���� (10us)
		digitalWrite(trig, LOW);
		delay(500);

		if (!flag && count>0)
		{	//trigger�� �Է��ϴ� �������� �ð� ����
			gettimeofday(&UTCtime_t1, NULL); // UTC ���� �ð� ���ϱ�(����ũ���ʱ���)
			disp_runtime(UTCtime_t2, UTCtime_t1);
		}
		else if(flag && count>0)
		{
			gettimeofday(&UTCtime_t2, NULL); // UTC ���� �ð� ���ϱ�(����ũ���ʱ���)
			disp_runtime(UTCtime_t1, UTCtime_t2);
		}
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

		if (count > 0)
			printf("runtime : %ld\n", UTCtime_r.tv_usec);

		// �Ÿ� ��� ����
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

