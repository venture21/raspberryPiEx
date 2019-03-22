#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 1

int main(void)
{
	char str;

	if (wiringPiSetup() == -1)
	{
		printf("Error:wiringPiSetup\n");
		return -1;
	}

	softPwmCreate(SERVO, 0, 200);

	while (1)
	{
		fputs("select 1,2,3,q:", stdout);
		scanf("%c", &str);
		getchar();
		if (str == '2')
			softPwmWrite(SERVO, 15);	//0 degree
		else if (str == '3')
			softPwmWrite(SERVO, 24);	//90 degree
		else if (str == '1')
			softPwmWrite(SERVO, 5);		//-90 degree
		else if (str == 'q')
			return 0;
	}
	return 0;
}