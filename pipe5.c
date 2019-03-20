#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int status;
	int fd;
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	pid_t fork_result;
	if (pipe(file_pipes) == 0) 
	{
		fork_result = fork();
		if (fork_result == -1) 
		{
			fprintf(stderr, "Fork failure");
			exit(EXIT_FAILURE);
		}

		// Child Process
		if (fork_result == 0) 
		{
			close(0);
			fd=dup(file_pipes[0]);
			printf("fd=%d\n",fd);
			close(file_pipes[0]);
			close(file_pipes[1]);
			execlp("od", "od", "-c", (char *)0);
			exit(EXIT_FAILURE);
		}
		// Parent Process
		else 
		{
			close(file_pipes[0]);
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			close(file_pipes[1]);
			printf("%d - wrote %d bytes\n", (int)getpid(), data_processed);
		}
		wait(&status);
	}
	exit(EXIT_SUCCESS);
}
