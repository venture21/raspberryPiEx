#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softTone.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "music.h"

#define	LED		1	// GPIO18
#define trig	4	// GPIO23
#define echo	5	// GPIO24
#define	piezo	6	// GPIO25
#define motor1	21   // GPIO5
#define motor2	22   // GPIO6
#define motor3	23   // GPIO13
#define motor4	24   // GPIO19

#define TOTAL	32	// 학교종의 전체 계이름의 수 
#define FWD		1
#define REW		0
#define START	1
#define STOP	0



struct Data
{
	int led_Value;
	float hc04_Dist;
};

//LED자원을 관리하기 위해 mutex변수를 선언한다.
pthread_mutex_t led_lock;
pthread_mutex_t hc04_lock;
pthread_mutex_t music_lock;
pthread_mutex_t motor_lock;

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
	data = *((struct Data *)arg);
	data.led_Value = 1;
	pinMode(LED, OUTPUT);
	while (1)
	{
		data = *((struct Data *)arg);
		ledControl(data.led_Value);
		printf("led=%d\n", data.led_Value);
		usleep(200000);
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
//=====================================================
// music Function
//=====================================================
// This function generates the square wave that makes
// the piezo speaker sound at a determinated frequency.
void beep(unsigned int note, unsigned int duration)
{
	//This is the semiperiod of each note.
	long beepDelay = (long)(1000000 / note);
	//This is how much time we need to spend on the note.
	long time = (long)((duration * 1000) / (beepDelay * 2));

	for (int i = 0; i < time; i++)
	{
		//1st semiperiod
		digitalWrite(piezo, HIGH);
		delayMicroseconds(beepDelay);
		//2nd semiperiod
		digitalWrite(piezo, LOW);
		delayMicroseconds(beepDelay);
	}

	//Add a little delay to separate the single notes
	digitalWrite(piezo, LOW);
	delay(20);

}

//The source code of the Imperial March from Star Wars
void musicPlay()
{
	if (pthread_mutex_trylock(&music_lock) != EBUSY) /* 임계 구역 설정 */
	{
		beep(a, 500);
		beep(a, 500);
		beep(f, 350);
		beep(cH, 150);

		beep(a, 500);
		beep(f, 350);
		beep(cH, 150);
		beep(a, 1000);
		beep(eH, 500);

		beep(eH, 500);
		beep(eH, 500);
		beep(fH, 350);
		beep(cH, 150);
		beep(gS, 500);

		beep(f, 350);
		beep(cH, 150);
		beep(a, 1000);
		beep(aH, 500);
		beep(a, 350);

		beep(a, 150);
		beep(aH, 500);
		beep(gHS, 250);
		beep(gH, 250);
		beep(fHS, 125);

		beep(fH, 125);
		beep(fHS, 250);

		delay(250);

		beep(aS, 250);
		beep(dHS, 500);
		beep(dH, 250);
		beep(cHS, 250);
		beep(cH, 125);

		beep(b, 125);
		beep(cH, 250);

		delay(250);

		beep(f, 125);
		beep(gS, 500);
		beep(f, 375);
		beep(a, 125);
		beep(cH, 500);

		beep(a, 375);
		beep(cH, 125);
		beep(eH, 1000);
		beep(aH, 500);
		beep(a, 350);

		beep(a, 150);
		beep(aH, 500);
		beep(gHS, 250);
		beep(gH, 250);
		beep(fHS, 125);

		beep(fH, 125);
		beep(fHS, 250);

		delay(250);

		beep(aS, 250);
		beep(dHS, 500);
		beep(dH, 250);
		beep(cHS, 250);
		beep(cH, 125);

		beep(b, 125);
		beep(cH, 250);

		delay(250);

		beep(f, 250);
		beep(gS, 500);
		beep(f, 375);
		beep(cH, 125);
		beep(a, 500);

		beep(f, 375);
		beep(c, 125);
		beep(a, 1000);
		pthread_mutex_unlock(&music_lock); /* 임계 구역 해제 */
	}
}


void *musicFunction(void *arg)
{
	pinMode(piezo, OUTPUT);
	digitalWrite(piezo, LOW);
	musicPlay();

	return NULL;
}

int main(int argc, char **argv)
{
	int err;
	pthread_t thread_LED;
	pthread_t thread_HC04;
	pthread_t thread_MUSIC;
	pthread_t thread_MOTOR;

	struct Data data;

	//Init
	wiringPiSetup();

	pthread_create(&thread_LED, NULL, ledFunction, (void*)&data);
	//pthread_create(&thread_HC04, NULL, hc04Function, (void*)&data);
	//pthread_create(&thread_MUSIC, NULL, musicFunction, (void*)&data);
	pthread_create(&thread_MOTOR, NULL, motorFunction, (void*)&data);

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
	pthread_join(thread_MOTOR, 0);

}
