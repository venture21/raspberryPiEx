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
#include "raspi.h"

#define	LED		1	// GPIO18
#define DEBUG


//LED자원을 관리하기 위해 mutex변수를 선언한다.
pthread_mutex_t led_lock;


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

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
	printf("sigHanlder\n");
	exit(0);
}

int main(int argc, char **argv)
{
	int err;
	int serv_sock, clnt_sock;
	pthread_t thread_LED;
	pid_t pid;
	int str_len;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	struct Data data, buf;

	//Init
	wiringPiSetup();

	signal(SIGINT, sigHandler);
	

	// STEP 1.
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// bind() error는 발생하지 않는다.
	//setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

	//STEP 2.
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	//STEP 3.
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected...");
		pid = fork();
		if (pid == -1)
		{
			close(clnt_sock);
			continue;
		}
		if (pid == 0)
		{
			close(serv_sock);

			//자식 프로세스를 생성한 뒤에 스레드를 생성한다.
			pthread_create(&thread_LED, NULL, ledFunction, (void*)&data);

			while ((str_len = read(clnt_sock, &buf, sizeof(buf))) != 0)
			{
				printf("buf.led_Value=%d\n", buf.led_Value);
				ledWrite(&data, buf.led_Value);
			}

			//자식 프로세스를 종료하기 전에 스레드를 종료한다.
			pthread_join(thread_LED, 0);

			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}

#ifdef DEBUG
	//TODO
	while (1)
	{
		if (data.led_Value == 1)
			ledWrite(&data, LOW);
		else
			ledWrite(&data, HIGH);
		sleep(1);
	}
#endif



	close(serv_sock);
	return 0;

}
