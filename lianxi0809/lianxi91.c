#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

//ADC0809
sbit START = P2^1;
sbit EOC = P2^2;
sbit OE = P2^4;
sbit CLK =  P2^0;

unsigned char code tab[] =  {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void DelayMS(uint ms)
{
	uchar i;
	while(ms--)
	{	
		for(i = 0;i<120;i++);
	}
}

void Display(uchar d)
{
	P1 = 0xfe;
	P0 = tab[value / 1000]; 
	delay(500);
	P1 = 0xfd;
	P0 = tab[(value / 100) % 10]|0x80; 
	delay(500);
	P1 = 0xfb;
	P0 = tab[(value / 10) % 10];  
	delay(500);
	P1 = 0xf7;
	P0 = tab[value % 10];            
	delay(500);
}

void main()
{
	TMOD = 0x02;
	TH0 = 0x14;
	TL0 = 0x00;
	IE = 0x82;
	TR0 = ;
	while(1)
	{
		START = 0;
		START = 1;
		START = 0;
		while(EOC == 0);
		OE = 1;
		Display(P3);
		OE = 0;
		
	}
}
void timer0() interrupt 1
{
	CLK = !CLK;
}