#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int


uint result = 0;
//ADC0809
sbit START = P2^1;
sbit EOC = P2^2;
sbit OE = P2^4;


unsigned char code tab[] =   {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(uint t) 
{
  while(t--);
}


void Display(float voltage) {
	uint value = (uint)(voltage*100); 
	P1 = 0xfe;
	P0 = ~tab[value / 1000]; 
	delay(500);
	P1 = 0xfd;
	P0 = ~tab[(value / 100) % 10]|0x80; 
	delay(500);
	P1 = 0xfb;
	P0 = ~tab[(value / 10) % 10];  
	delay(500);
	P1 = 0xf7;
	P0 = ~tab[value % 10];            
	delay(500);
}


// ADC0809
void ADC0809() {
	START = 0;
	delay(500);
	START = 1;
	delay(500);
	START = 0;
	
	
	while(EOC!=1);
	delay(500);
	OE = 1;
	delay(500);
	result = P3;
	
	OE = 0;
	
}

void main() {
	float dat;
	
	while(1) 
	{
		ADC0809();
		dat = (5.0*result)/256.0;
		Display(dat);	
		OE = 0;
	}
}