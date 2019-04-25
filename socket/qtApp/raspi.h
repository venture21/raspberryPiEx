#define WR_LED	1
#define RD_HC04	2
#define WR_DIST	3
#define RD_IMG	4
#define WR_IMG	5

#define FILENAME "raspi.jpg"
#define BUF_SIZE 1024

void ledControl(int value);

static void *clnt_connection(void *arg);
int sendData(int fd, FILE* fp, char *ct, char *file_name);
void sendOk(FILE* fp);
void sendError(FILE* fp);

struct Data
{
	int cmd;
	int endFlag;
	int led_Value;
	float hc04_dist;
};