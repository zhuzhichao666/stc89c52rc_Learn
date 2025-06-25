#include<reg51.h>

sbit LED = P0^0;
unsigned char Y[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};


void delay(unsigned int t)
{
	while(t--);
}

void main()
{
	unsigned char i;
	EA = 1;
	IT0 = 0;
	IT1 = 0;
	EX0 = 1;
	EX1 = 1;
	PX0 = 0;
	PX1 = 1;
	while(1)
	{
		for(i = 0;i<10;i++){
			P2 = Y[i];
			delay(50000);
		}
	}
}

void int0() interrupt 0
{
	unsigned char j, temp = 0x01;
	for(j = 0; j < 8; j++)
	{
		P1 = ~temp;
		delay(50000);
		temp <<= 1;
	}
}
void int1() interrupt 2
{
	unsigned char k ;
	for(k = 0;k<10;k++)
	{
		LED = 1;
		delay(50000);
		LED = 0;
		delay(50000);
	}
}