#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	unsigned long clntAddr;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	char message[]="Hello World!";
	
	if (argc != 2)
	{
		// ./helloServer [PORT_NUM]
		printf("Usage : %s <PORT>\n", argv[0]);
		exit(1);
	}

	//STEP 1. socket을 생성한다. file의  open과 동일
	// PF_INET : IP v4
	// SOCK_STREAM : TCP
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	//STEP 2. 접속할 서버의 IP주소, 포트번호, 프로토콜을 정의
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//STEP 3. BIND
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
		
	//STEP 4. listen() 5의 의미는 동시접속을 처리할 수 있는 버퍼의 크기 
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");

	//클라이언트의 주소값을 확인한다.
	printf("Client Addr : %s\n", inet_ntoa(clnt_addr.sin_addr));

	//STEP 5. 데이터 송신
	write(clnt_sock, message, sizeof(message));

	// 서버의 소켓을 먼저 닫을 경우 bind()에러가 발생할 수 있다.
	// 클라이언트보다 늦게 소켓을 종료하기 위해 sleep!
	sleep(2);

	// STEP 6. 소켓 종료
	close(clnt_sock);
	close(serv_sock);

	return 0;
}

