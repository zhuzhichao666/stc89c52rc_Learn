#include<reg51.h>

unsigned char tube[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(unsigned int t)
{
	while(t--);
}

unsigned int m = 9427;

void main()
{
	while(1)
	{
		P3 = 0x0e;
		P2 = tube[m/1000];
		delay(500);
		
		P3 = 0x0d;
		P2 = tube[m%1000/100];
		delay(500);
		
		P3 = 0x0b;
		P2 = tube[m%100/10];
		delay(500);
		
		P3 = 0x07;
		P2 = tube[m%10];
		delay(500);
	}
}