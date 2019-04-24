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
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define	LED		1	// GPIO18


struct Data
{
	int led_Value;
};

//LED자원을 관리하기 위해 mutex변수를 선언한다.
pthread_mutex_t led_lock;

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

static void sigHandler(int signum)
{
	digitalWrite(piezo, LOW);
	exit(0);
}

int main(int argc, char **argv)
{
	int err;
	pthread_t thread_LED;

	struct Data data;

	//Init
	wiringPiSetup();

	signal(SIGINT, sigHandler);
	pthread_create(&thread_LED, NULL, ledFunction, (void*)&data);

	//TODO

	pthread_join(thread_LED, 0);
	
}
