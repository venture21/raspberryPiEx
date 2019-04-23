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

	//STEP 1. socket�� �����Ѵ�. file��  open�� ����
	// PF_INET : IP v4
	// SOCK_STREAM : TCP
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//STEP 2. ������ ������ IP�ּ�, ��Ʈ��ȣ, ���������� ����
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//STEP 3. ���� ��û
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error!");
	}

	// STEP 4. ������ ����
	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1)
		error_handling("read() error!");

	printf("Message from server: %s\n", message);

	// STEP 5. ���� ����
	close(sock);

	return 0;
}

