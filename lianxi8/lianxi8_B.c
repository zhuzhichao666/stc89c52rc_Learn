#include<reg51.h>

void delay(unsigned int t)
{
	while(t--);
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
	
	P0 = 0x11;
	
	
	while(1);
}

void BToA() interrupt 4
{
	if(RI)
	{
		unsigned char Data,FData;
		Data = SBUF;
		FData = ~Data;
		P0 = Data;
		SBUF = FData;
		while(TI == 0);
		TI = 0;
		RI = 0;
		delay(5000);
	}
}