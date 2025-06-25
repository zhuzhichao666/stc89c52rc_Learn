#include<reg51.h>

unsigned char shu[] = {0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void delay(unsigned int t)
{
	while(t--);
}

void main()
{
	while(1)
	{
		P2 = 0x01;
		P0 = shu[1];
		delay(500);
		P2 = 0x02;		
		P0 = shu[2];
		delay(500);
		P2 = 0x04;
		P0 = shu[3];
		delay(500);
		P2 = 0x08;
		P0 = shu[4];
		delay(500);
	}
}