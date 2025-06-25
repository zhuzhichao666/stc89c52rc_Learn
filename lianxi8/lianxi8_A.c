#include<reg51.h>

void delay(unsigned int t)
{
	while(t--);
}

void Write(unsigned char w)
{
	SBUF = w;
	while(TI==0);
	TI = 0;
	delay(50000);
}

void main()
{
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	SCON = 0x50;//允许接收数据且工作方式1
	EA = 1;
	ES = 1;
	TR1 = 1;
	while(1)
	{
		Write(0x55);
		Write(0x11);
		Write(0x22);
	}
}

void BToA() interrupt 4
{
	if(RI)
	{
		unsigned char Data;
		Data = SBUF;
		P0 = Data;
		RI = 0;
	}
}