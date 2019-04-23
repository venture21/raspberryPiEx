#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "mymsg.h"

// For IPC function
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(void)
{
	int running = 1;
	struct myData person[10];
	struct msqid_ds msqstat;  //남은 메시지큐의 메시지 갯수를 확인하기 위해
	int msgid;
	int personNum = 0;

	//step0. ftok();

	//step1. msgget 
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if (msgid == -1)
	{
		fprintf(stderr, "Error:msgget failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while (running)
	{
		//step2. msgrcv
		if (msgrcv(msgid, &person[personNum], sizeof(person) - sizeof(long), 0, 0) == -1)
		{
			fprintf(stderr, "Error:msgrcv failed : %d\n", errno);
			exit(EXIT_FAILURE);
		}

		if (person[personNum].msgType == 2)
			running = 0;
		else
		{
			printf("Receive Data Person[%d]\n", personNum);
			printf("Name:%s", person[personNum].name);
			printf("Height:%d\n", person[personNum].height);
			printf("Weight:%d\n", person[personNum].weight);
		}

		if (msgctl(msgid, IPC_STAT, &msqstat) == -1)
		{
			perror("Fail:msgctl()");
			exit(1);
		}
		printf("remain message count:%d\n", msqstat.msg_qnum);
		if (msqstat.msg_qnum > 0)
			running = 1;
		personNum++;
	}

	//step3. msgctl IPC_RMID
	if (msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "Error:msgctl IPC_RMID failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
	

