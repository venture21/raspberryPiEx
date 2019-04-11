//=================================
// 7-Segment 5611bs (Common Anode)
//=================================

#include <wiringPi.h>

#define PIN_A	27	//GPIO16
#define PIN_B	26	//GPIO12
#define PIN_C	23	//GPIO13
#define PIN_D	24	//GPIO19
#define PIN_E	25	//GPIO26
#define PIN_F	28	//GPIO20
#define PIN_G	29	//GPIO21
#define PIN_DP	22	//GPIO6


int DISP[10][8] = {	{0,0,0,0,0,0,1,1},
			{1,0,0,1,1,1,1,1},
			{0,0,1,0,0,1,0,1},
			{0,0,0,0,1,1,0,1},
			{1,0,0,1,1,0,0,1},
			{0,1,0,0,1,0,0,1},
			{0,1,0,0,0,0,0,1},
			{0,0,0,1,1,1,1,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,1,0,0,1}
			};

int PIN_NUM[8] = {PIN_A,PIN_B,PIN_C,PIN_D,PIN_E,PIN_F,PIN_G,PIN_DP};


void disp_segment(int value)//0
{
	int i;
	for(i=0;i<9;i++)
		digitalWrite(PIN_NUM[i], DISP[value][i]);
}

int main()
{
	int i;
	int digit;
	wiringPiSetup();
	{
		for (i = 22; i <= 29; i++)
			pinMode(i, OUTPUT);
	}
	while (1)
	{
		for (digit =0; digit <10; digit++)
		{
			disp_segment(digit);
			delay(1000);
		}
	};
}
