#include <reg51.h>

sbit bo = P2^0;

unsigned char flag = 1,count = 0;

void main()
{
	TMOD = 0x01;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
	TH0 = 0xFC;
  TL0 = 0x18;
	while(1);
}

void time0() interrupt 1
{
	TH0 = 0xFC;
	TL0 = 0x18;
  count++;
	if(flag == 1)
	{
		if(count >= 2)
		{
			bo = 0;
			count = 0;
			flag = 0;
		}
	}
	if(flag == 0)
	{
		if(count >= 3)
		{
			bo = 1;
			count = 0;
			flag = 1;
		}
	}
}
