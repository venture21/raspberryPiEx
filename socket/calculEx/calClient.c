#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DEBUG

struct calcul
{
	int opnd_cnt;
	int opnd[4];
	char oper;
};


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock; //fd
	int i;
	struct calcul cal;
	struct sockaddr_in serv_addr;
	int str_len;

	if (argc != 3)
	{
		// ./calClient [SERVER_IPADDR] [PORT_NUM]
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	//STEP 1. socket을 생성한다. file의  open과 동일
	// PF_INET : IP v4
	// SOCK_STREAM : TCP
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//STEP 2. 접속할 서버의 IP주소, 포트번호, 프로토콜을 정의
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//STEP 3. 접속 요청
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error!");
	}

	printf("Operand count:");
	scanf("%d", &cal.opnd_cnt);

	for (i = 0; i < cal.opnd_cnt; i++)
	{
		printf("Operand %d:",i + 1);
		scanf("%d", &cal.opnd[i]);
	}
	getchar();
	printf("Operation:");
	scanf("%c",&cal.oper);

#ifdef DEBUG
	printf("opnd_cnt:%d\n", cal.opnd_cnt);
	printf("opnd[0]:%d\n", cal.opnd[0]);
	printf("opnd[1]:%d\n", cal.opnd[1]);
	printf("opnd[2]:%d\n", cal.opnd[2]);
	printf("oper:%d\n", cal.oper);
#endif
	//STEP 5. 데이터 송신
	write(sock, &cal, sizeof(cal));


	// STEP 5. 소켓 종료
	close(sock);

	return 0;
}