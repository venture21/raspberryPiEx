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

//LED�ڿ��� �����ϱ� ���� mutex������ �����Ѵ�.
pthread_mutex_t led_lock;
pthread_mutex_t hc04_lock;

void ledControl(int value)
{
	// LED�� ���� MUTEX����
	if (pthread_mutex_trylock(&led_lock) != EBUSY)
	{
		digitalWrite(LED, value);
		pthread_mutex_unlock(&led_lock);
	}
	return;
}

void* ledFunction(void *arg)
{
	pinMode(LED, OUTPUT);
	ledControl(1);
	return;
}


int main(int argc, char **argv)
{
	int err;
	pthread_t thread_LED, thread_HC04;

	//Init
	wiringPiSetup();

	pthread_create(&thread_LED, NULL, ledFunction, NULL);
	pthread_create(&)
	pthread_join(thread_LED, 0);

}
