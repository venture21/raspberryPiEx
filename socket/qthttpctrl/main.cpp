#include <QApplication>

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

#include <QApplication>

#include "raspi.h"
#include "withQt.h"

#define	LED		1	// GPIO18
#define trig	4	// GPIO23
#define echo	5	// GPIO24

#define PORTNUM 5000
#define DEBUG
#define MAX_CLNT 256

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

struct Data buf;
struct Data data;

static pthread_t t_id;

//LED자원을 관리하기 위해 mutex변수를 선언한다.
pthread_mutex_t led_lock;
pthread_mutex_t hc04_lock;

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
		data.led_Value = value;
		digitalWrite(LED, value);
		pthread_mutex_unlock(&led_lock);
	}
	return;
}

void* ledFunction(void *arg)
{
	pinMode(LED, OUTPUT);

	while (!data.endFlag)
	{
		ledControl(data.led_Value);
		//printf("led=%d\n", data.led_Value);
		usleep(200000);
	}

}

//=====================================================
// HC_SR04 Function
//=====================================================
float hc04Control(void)
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
		usleep(10000);
		//printf("distance:%f\n", distance);
	}
	return distance;
}

void* hc04Function(void *arg)
{
	float ret;
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	digitalWrite(trig, LOW);
	while (!data.endFlag)
	{
		ret = hc04Control();
		data.hc04_dist = ret;

	}
	return NULL;
}


void *clnt_connection(void *arg)
{
	/* 스레드를 통해서 넘어온 arg를 int 형의 파일 디스크립터로 변환한다. */
	int clnt_sock = *((int*)arg), clnt_fd;
	FILE *clnt_read, *clnt_write;
	char reg_line[BUFSIZ], reg_buf[BUFSIZ];
	char method[10], ct[BUFSIZ], type[BUFSIZ];
	char file_name[256], file_buf[256];
	char* type_buf;
	int i = 0, j = 0, len = 0;

	/* 파일 디스크립터를 FILE 스트림으로 변환한다. */
	clnt_read = fdopen(clnt_sock, "r");
	clnt_write = fdopen(dup(clnt_sock), "w");
	clnt_fd = clnt_sock;

	/* 한 줄의 문자열을 읽어서 reg_line 변수에 저장한다. */
	fgets(reg_line, BUFSIZ, clnt_read);

	/* reg_line 변수에 문자열을 화면에 출력한다. */
	fputs(reg_line, stdout);

	/* ‘ ’ 문자로 reg_line을 구분해서 요청 라인의 내용(메소드)를 분리한다. */
	strcpy(method, strtok(reg_line, " "));
	if (strcmp(method, "POST") == 0) { /* POST 메소드일 경우를 처리한다. */
		sendOk(clnt_write); /* 단순히 OK 메시지를 클라이언트로 보낸다. */
		fclose(clnt_read);
		fclose(clnt_write);

		return (void*)NULL;
	}
	else if (strcmp(method, "GET") != 0) { /* GET 메소드가 아닐 경우를 처리한다. */
		sendError(clnt_write); /* 에러 메시지를 클라이언트로 보낸다. */
		fclose(clnt_read);
		fclose(clnt_write);

		return (void*)NULL;
	}

	strcpy(file_name, strtok(NULL, " ")); /* 요청 라인에서 경로(path)를 가져온다. */
	if (file_name[0] == '/') { /* 경로가 ‘/’로 시작될 경우 /를 제거한다. */
		for (i = 0, j = 0; i < BUFSIZ; i++) {
			if (file_name[0] == '/') j++;
			file_name[i] = file_name[j++];
			if (file_name[i + 1] == '\0') break;
		};
	}

	/* 라즈베리 파이를 제어하기 위한 HTML 코드를 분석해서 처리한다. */
	if (strstr(file_name, "?") != NULL) {
		char optLine[32];
		char optStr[4][16];
		char opt[8], var[8];
		char* tok;
		int i, count = 0;

		strcpy(file_name, strtok(file_name, "?"));
		strcpy(optLine, strtok(NULL, "?"));

		/* 옵션을 분석한다. */
		tok = strtok(optLine, "&");
		while (tok != NULL) {
			strcpy(optStr[count++], tok);
			tok = strtok(NULL, "&");
		};

		/* 분석한 옵션을 처리한다. */
		for (i = 0; i < count; i++) {
			strcpy(opt, strtok(optStr[i], "="));
			strcpy(var, strtok(NULL, "="));
			printf("%s = %s\n", opt, var);
			if (!strcmp(opt, "led") && !strcmp(var, "On")) { /* LED를 켠다. */
				ledControl(1);
			}
			else if (!strcmp(opt, "led") && !strcmp(var, "Off")) { /* LED를 끈다. */
				ledControl(0);
			}
		};
	}

	/* 메시지 헤더를 읽어서 화면에 출력하고 나머지는 무시한다. */
	do {
		fgets(reg_line, BUFSIZ, clnt_read);
		fputs(reg_line, stdout);
		strcpy(reg_buf, reg_line);
		type_buf = strchr(reg_buf, ':');
	} while (strncmp(reg_line, "\r\n", 2)); /* 요청 헤더는 ‘\r\n’으로 끝난다. */

	/* 파일의 이름을 이용해서 클라이언트로 파일의 내용을 보낸다. */
	strcpy(file_buf, file_name);
	sendData(clnt_fd, clnt_write, ct, file_name);

	fclose(clnt_read); /* 파일의 스트림을 닫는다. */
	fclose(clnt_write);

	pthread_exit(0); /* 스레드를 종료시킨다. */

	return (void*)NULL;
}

int sendData(int fd, FILE* fp, char *ct, char *file_name)
{
	/* 클라이언트로 보낼 성공에 대한 응답 메시지 */
	char protocol[] = "HTTP/1.1 200 OK\r\n";
	char server[] = "Server:Netscape-Enterprise/6.0\r\n";
	char cnt_type[] = "Content-Type:text/html\r\n";
	char end[] = "\r\n"; /* 응답 헤더의 끝은 항상 \r\n */
	char buf[BUFSIZ];
	int len;

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_type, fp);
	fputs(end, fp);
	fflush(fp);

	/* 파일 이름이 capture.bmp인 경우 이미지를 캡처한다. */
	if (!strcmp(file_name, "capture.bmp"))
		captureImage(fd);

	fd = open(file_name, O_RDWR); /* 파일을 연다. */
	do {
		len = read(fd, buf, BUFSIZ); /* 파일을 읽어서 클라이언트로 보낸다. */
		fwrite(buf, len, sizeof(char), fp);
	} while (len == BUFSIZ);

	fflush(fp);

	close(fd); /* 파일을 닫는다. */

	return 0;
}

void sendOk(FILE* fp)
{
	/* 클라이언트에 보낼 성공에 대한 HTTP 응답 메시지 */
	char protocol[] = "HTTP/1.1 200 OK\r\n";
	char server[] = "Server: Netscape-Enterprise/6.0\r\n\r\n";

	fputs(protocol, fp);
	fputs(server, fp);
	fflush(fp);
}

void sendError(FILE* fp)
{
	/* 클라이언트로 보낼 실패에 대한 HTTP 응답 메시지 */
	char protocol[] = "HTTP/1.1 400 Bad Request\r\n";
	char server[] = "Server: Netscape-Enterprise/6.0\r\n";
	char cnt_len[] = "Content-Length:1024\r\n";
	char cnt_type[] = "Content-Type:text/html\r\n\r\n";

	/* 화면에 표시될 HTML의 내용 */
	char content1[] = "<html><head><title>BAD Connection</tiitle></head>";
	char content2[] = "<body><font size=+5>Bad Request</font></body></html>";

	printf("send_error\n");
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fputs(content1, fp);
	fputs(content2, fp);
	fflush(fp);
}

void* userThread(void *arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	int read_cnt;
	int wrDone = 0;
	pthread_t t_id[2];
	FILE *fp;
	char filebuf[BUF_SIZE];

	while ((str_len = read(clnt_sock, &buf, sizeof(buf))) != 0)
	{
		switch (buf.cmd)
		{
		case WR_LED: data.led_Value = buf.led_Value;
			printf("data.led_Value=%d\n", data.led_Value);
			break;
		case RD_HC04:
			data.cmd = WR_DIST;
			write(clnt_sock, &data, sizeof(data));
			printf("data.dist:%f\n", data.hc04_dist);
			break;
		case RD_IMG:
			data.cmd = WR_IMG;
			write(clnt_sock, &data, sizeof(data));
			fp = fopen(FILENAME, "rb");
			while (1)
			{
				read_cnt = fread((void*)filebuf, 1, BUF_SIZE, fp);
				// 남아있는 파일의 사이즈가 BUF_SIZE보다 작을 경우
				if (read_cnt < BUF_SIZE)
				{
					write(clnt_sock, filebuf, read_cnt);
					break;
				}
				// 남아있는 파일의 사이즈가 BUF_SIZE보다 큰 경우
				write(clnt_sock, filebuf, BUF_SIZE);
			}
			fclose(fp);
			wrDone = 0;
			printf("File Write is done\n");
			break;
		default:
			break;
		}
	}
	data.endFlag = 1;

	printf("userThread is end\n");
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++)   // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (i++ < clnt_cnt - 1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
}



static void sigHandler(int signum)
{
	printf("sigHanlder\n");
	exit(0);
}

void* webServer(void *arg)
{
	int err;
	int serv_sock, clnt_sock;
	pthread_t thread_LED;
	pthread_t t_id[3];
	int str_len;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	data.endFlag = 0;
	//Init
	wiringPiSetup();

	signal(SIGINT, sigHandler);

	// STEP 1. socket생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(PORTNUM);

	// bind() error는 발생하지 않는다.
	//setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

	//STEP 2. bind : IP주소와 포트에 bind하기
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	//STEP 3. 서버 소켓의 연결 요청 대기 큐를 5개로 설정
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	pthread_create(&t_id[0], NULL, ledFunction, 0);
	pthread_create(&t_id[1], NULL, hc04Function, 0);

	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);

		// 사용자의 요청에 대응하기 위해 클라이언트당 1개의 스레드 생성
		pthread_create(&t_id[2], NULL, userThread, (void*)&clnt_sock);
		pthread_detach(t_id[2]);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}

	pthread_detach(t_id[0]);
	pthread_detach(t_id[1]);

	close(serv_sock);
	return 0;
}

int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	PiWidget *widget = new PiWidget();
	widget->resize(150, 250);
	widget->show();

	pthread_create(&t_id, NULL, webServer, NULL);

	return app.exec();
}

