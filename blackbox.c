#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


#define DEBUG
#define VIDEOTIME	60000	//unit:msec

const char *MMOUNT = "/proc/mounts";

struct f_size
{
	long blocks;	// 전체 블럭 수
	long avail;		// 사용가능 블럭 수
};

typedef struct
{
	FILE *fp;               // 파일 스트림 포인터    
	char devname[80];       // 장치 이름
	char mountdir[80];      // 마운트 디렉토리 이름
	char fstype[12];        // 파일 시스템 타입
	long f_type;
	long f_blocks;	    // Total Block count
	long f_bavail;	    // unused block count(Normal User)
	struct f_size size;     // 파일 시스템의 총크기/사용율 
} MOUNTP;

MOUNTP *dfopen()
{
	MOUNTP *MP;

	// /proc/mounts 파일을 연다.
	MP = (MOUNTP *)malloc(sizeof(MOUNTP));
	if (!(MP->fp = fopen(MMOUNT, "r")))
	{
		return NULL;
	}
	else
		return MP;
}


MOUNTP *dfget(MOUNTP *MP)
{
	char buf[256];
	char *bname;
	char null[16];
	struct statfs lstatfs;
	struct stat lstat;
	int is_root = 0;

	// /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
	//fgets는 /proc/mounts파일의 정보를 1라인씩 읽어온다.
	while (fgets(buf, 256, MP->fp))
	{
		//is_root = 0;
		sscanf(buf, "%s%s%s", MP->devname, MP->mountdir, MP->fstype);
		if (strcmp(MP->mountdir, "/") == 0) is_root = 1;
		if (stat(MP->devname, &lstat) == 0 || is_root)
		{
			if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)
			{
				// 파일시스템의 총 할당된 크기와 사용량을 구한다.        
				statfs(MP->mountdir, &lstatfs);
				MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize / 1024);
				MP->size.avail = lstatfs.f_bavail * (lstatfs.f_bsize / 1024);
				return MP;
			}
		}
	}
	rewind(MP->fp);
	return NULL;
}

int dfclose(MOUNTP *MP)
{
	fclose(MP->fp);
}




int main(int argc, char **argv)
{
	time_t UTCtime;
	struct tm *tm;
	char dirNamebuf[BUFSIZ];
	char fileNamebuf[BUFSIZ];
	char tempbuf[20];
	char cmdbuf[BUFSIZ];
	int width;
	int height;
	int status;
	float remain;
	pid_t pid;


	MOUNTP *MP;

	//argv[1]==0, FHD
	if (!strcmp("0",argv[1]))
	{
		width = 1920;
		height = 1080;
	}
	//argv[1]==1, HD
	else if (!strcmp("1", argv[1]))
	{
		width = 1280;
		height = 720;
	}
	else
	{
		printf("Error: argv[1] is not valid\n");
		return -1;
	}

#ifdef DEBUG
	printf("width:%d, height:%d\n", width, height);
#endif

	while (1)
	{
		//커널로부터 시간을 읽어온다.
		time(&UTCtime);

		//tm구조체 : 분해된 시간으로 저장
		tm = localtime(&UTCtime);

		//디렉토리명을 생성
		strftime(dirNamebuf, sizeof(dirNamebuf), "%Y%m%d%H", tm);

		//파일명 생성
		strftime(tempbuf, sizeof(tempbuf), "%Y%m%d_%H%M%S.h264", tm);
		sprintf(fileNamebuf, "%s/%s", dirNamebuf, tempbuf);

#ifdef DEBUG
		printf("dirNamebuf:%s\n", dirNamebuf);
		printf("fileNamebuf:%s\n", fileNamebuf);
#endif

		struct dirent *de = NULL;

		if ((MP = dfopen()) == NULL)
		{
			printf("Error:dfopen()\n");
			return -1;
		}

		dfget(MP);

		remain = ((((float)MP->size.avail*100)/(float)MP->size.blocks));
#ifdef DEBUG
		printf("size.blocks:%ld\n", MP->size.blocks);
		printf("size.avail:%ld\n", MP->size.avail);
		printf("remain(%):%f\n", remain);
#endif
		//남아있는 디스크 공간이 5%미만이라면
		//가장 오래된 디렉토리를 삭제한다.
		if (remain < 5)
		{
			//TODO : 가장 오래된 디렉토리를 찾아서 삭제 추가
		}
		dfclose(MP);

		//현재 시간으로 디렉토리 생성
		if (mkdir(dirNamebuf, 0666) == -1)
		{
			if (errno != EEXIST)
				return -1;
		}
		chmod(dirNamebuf, 0777);

		sprintf(cmdbuf, "raspivid -w %d -h %d -t %d -o %s", \
				width, height, VIDEOTIME, fileNamebuf);
#ifdef DEBUG
		printf("cmdbuf:%s\n", cmdbuf);
#endif
		pid = fork();
		
		//부모 프로세스
		if (pid > 0)
		{
			// none
		}
		else if (pid == 0)
		{
			system(cmdbuf);
			exit(1);
		}

		wait(&status);
	}



	return 0;
}