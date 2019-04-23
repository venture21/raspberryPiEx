/* Here's the receiver program. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msg_st 
{
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main()
{
	int running = 1;
	int msgid;
	int ndx;

	struct   msqid_ds msqstat;
	struct my_msg_st some_data;
	/* First, we set up the message queue. */
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if (msgid == -1) 
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	/*  Then the messages are retrieved from the queue, until an
   		end message is encountered.
		Lastly, the message queue is deleted. */

	while(running) 
	{
		if (msgrcv(msgid, &some_data, sizeof(some_data)-sizeof(long),0, 0) == -1) 
		{
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s", some_data.some_text);
		
		if (strncmp(some_data.some_text, "end", 3) == 0) 
		{
			running = 0;
		}
	}

	if (-1 == msgctl(msgid, IPC_STAT, &msqstat))
	{
		perror("Fail:msgctl()");
		exit(1);
	}

	printf("remain message count: %d\n", msqstat.msg_qnum);
	for (ndx = 0; ndx < msqstat.msg_qnum; ndx++)
	{
		if (-1 == msgrcv(msgid, &some_data, sizeof(some_data) - sizeof(long), 0, 0))
		{
			perror("Fail:msgrcv()");
			exit(1);
		}
		printf("You wrote: %s", some_data.some_text);
	}

	if (msgctl(msgid, IPC_RMID, 0) == -1) 
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
