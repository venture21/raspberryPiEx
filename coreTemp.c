#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define buf_size 10
#define CPU_TEMP	 "/sys/class/thermal/thermal_zone0/temp"
#define CPU_CUR_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define CPU_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

#define CUR_FREQ	1
#define MIN_FREQ	2
#define MAX_FREQ	3


float readCpuTemp(void)
{
	int fd;
	float temp;
	char buf[buf_size];

	// CPU 온도를 읽어오기전에 문자열 버퍼 초기화
	memset(buf, 0, buf_size);

	fd = open(CPU_TEMP, O_RDONLY);
	if (fd < 3)
	{
		printf("Erro : Can't open temp file\n");
	}

	read(fd, buf, 5);
	temp = atoi(buf);
	temp /= 1000;

	close(fd);

	return temp;
}


float readCpuFreq(int freq_type)
{
	int fd;
	float freq;
	char buf[buf_size];

	// CPU속도를 읽어오기전에 문자열 버퍼 초기화
	memset(buf, 0, buf_size);

	switch(freq_type)
	{
		case CUR_FREQ: fd = open(CPU_CUR_FREQ, O_RDONLY);	break;
		case MIN_FREQ: fd = open(CPU_MIN_FREQ, O_RDONLY);	break;
		case MAX_FREQ: fd = open(CPU_MAX_FREQ, O_RDONLY);	break;
	}
	
	if (fd < 3)
	{
		printf("Error : Can't open freq file\n");
	}

	read(fd, buf, 8);
	freq = atoi(buf);
	freq /= 1000;

	close(fd);

	return freq;
}


int main(int argc, char **argv)
{

	float temp;
	float cpu_freq;
	
	while (1)
	{
		temp = readCpuTemp();
		printf("Temperature: %6.3fC.\n", temp);

		cpu_freq = readCpuFreq(CUR_FREQ);
		printf("Cpu current Freq: %6.3fMHz\n", cpu_freq);
		cpu_freq = readCpuFreq(MAX_FREQ);
		printf("Cpu max Freq: %6.3fMHz\n", cpu_freq);
		cpu_freq = readCpuFreq(MIN_FREQ);
		printf("Cpu min Freq: %6.3fMHz\n", cpu_freq);




		sleep(1);


	}
}

