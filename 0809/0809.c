#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int 
	
#define LED P0
#define out P3

sbit START = P2^1;
sbit EOC = P2^2;
sbit OE = P2^4;

void main()
{
	uchar temp;
	while(1)
	{
		START = 0;
		START = 1;
		START = 0;
		while(1)
		{
			while(EOC==0);
			OE = 1;
			temp = out;
			OE = 0;
			LED = temp;
		}
	}
	
}