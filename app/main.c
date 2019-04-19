#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	LED		1	//GPIO18
#define trig	4	//GPIO23
#define echo	5	//GPIO24

struct Data
{
	volatile int led_Value;
	volatile float hc04_Dist;
};


//LED자원을 관리하기 위해 mutex변수를 선언한다.
pthread_mutex_t led_lock;
pthread_mutex_t hc04_lock;

//=====================================================
// LED Function
//=====================================================
void ledWrite(struct Data *data, int value)
{
	data->led_Value = value;
}

void ledControl(int value)
{
	// LED를 위한 MUTEX구현
	if (pthread_mutex_trylock(&led_lock) != EBUSY)
	{
		digitalWrite(LED, value);
		pthread_mutex_unlock(&led_lock);
	}
	return;
}

void* ledFunction(void *arg)
{
	struct Data data;
	
	data.led_Value = 1;
	pinMode(LED, OUTPUT);
	while (1)
	{
		data = *((struct Data *)arg);
		ledControl(data.led_Value);
		printf("led=%d\n", data.led_Value);
		usleep(100000);
	}
	return NULL;
}

//=====================================================
// HC_SR04 Function
//=====================================================
int hc04Control(void)
{
	int startTime, endTime;
	float distance;

	// LED를 위한 MUTEX구현
	if (pthread_mutex_trylock(&hc04_lock) != EBUSY)
	{
		// Trig신호 생성 (10us)
		digitalWrite(trig, HIGH);
		usleep(10);					//wiringPi : delayMicroseconds(10); 
		digitalWrite(trig, LOW);

		// Echo back이 '1'이면 시작시간 기록 
		while (digitalRead(echo) == 0);
		//wiringPi : wiringPiSetup이후의 시간을 마이크로초로 측정하는 함수
		startTime = micros();

		// Echo back이 '1'이면 대기 
		while (digitalRead(echo) == 1);
		// Echo back이 '0'이면 종료시간 기록 
		endTime = micros();

		distance = (endTime - startTime) / 58;

		pthread_mutex_unlock(&hc04_lock);
	}
	return distance;
}

void* hc04Function(void *arg)
{
	int ret;
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	digitalWrite(trig, LOW);
	while (1)
	{
		ret = hc04Control();
		printf("distance:%d\n", ret);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int err;
	pthread_t thread_LED, thread_HC04;
	struct Data data;

	//Init
	wiringPiSetup();

	pthread_create(&thread_LED, NULL, ledFunction, (void*)&data);
	pthread_create(&thread_HC04, NULL, hc04Function, (void*)&data);

	while (1)
	{
		if (data.led_Value == 1)
			ledWrite(&data, LOW);
		else
			ledWrite(&data, HIGH);
		sleep(1);
	}


	pthread_join(thread_LED, 0);
	pthread_join(thread_HC04, 0);

}
