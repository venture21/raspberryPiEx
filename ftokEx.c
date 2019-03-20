
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

// File TO Key
// key_t ftok(const char *pathname,int proj_id)

int main(int argc, char **argv)
{
	key_t msgKey;

	msgKey = ftok("/home/pi/raspberryPiEx", 'A');
	printf("ftok_key=%d\n", msgKey);

	return 0;
}


