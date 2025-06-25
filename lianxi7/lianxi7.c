#include <reg51.h>

sbit stcp = P2^0;

#define uchar unsigned char 
#define uint unsigned int

uchar tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void delay(uint t)
{
	while(t--);
}

void display(uchar dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
	stcp = 0;
	stcp = 1;
}

void main()
{
	SCON = 0x00;
	while(1)
	{
		P1 = 0x01;
		display(tab[0]);
		delay(500);
		
		P1 = 0x02;
		display(tab[1]);
		delay(500);
		
		P1 = 0x04;
		display(tab[2]);
		delay(500);
	
		P1 = 0x08;
		display(tab[3]);
		delay(500);
	}
}