#include <reg51.h>

unsigned char flag = 0;
unsigned char num = 0;
unsigned char flag1 = 0;
unsigned long int cnt = 0;
unsigned long int count = 0;
unsigned int pwm = 0;
unsigned int speed = 0;

unsigned char tab[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void delay(unsigned int t)
{
	while(t--);
}

void display(unsigned int speed)
{
	P0 = 0x0e;
	P2 =  tab[speed/1000];
	delay(500);
	
	P0 = 0x0d;
	P2 =  tab[speed%1000/100];
	delay(500);
	
	P0 = 0x0b;
	P2 =  tab[speed%100/10];
	delay(500);
	
	P0 = 0x07;
	P2 =  tab[speed%10];
	delay(500);
}

void main()
{
	TMOD = 0x15;//T0计数器T1定时器
	
	TH0 = 0;
	TL0 = 0;//计数器初值
	TH1 = 0x3c;
	TL1 = 0xaf;//定时器初值50ms
	
	TR0 = 1;
	TR1 = 1;//
	
	ET0 = 1;
	ET1 = 1;
	EA = 1;//tcon
	
	while(1)
	{
		if(flag1 == 1)
		{
			flag1 = 0;
			count = TH0*256+TL0;
			TH0 = 0;
			TL0 = 0;
			cnt = flag*65535+count;
			flag = 0;
			pwm = cnt/100;
			speed = pwm*2;
			
			TR0 = 1;
			TR1 = 1;
		} 
		display(speed);
	}
}

void timer0() interrupt 1
{
	flag++;//计数器每65535+1
}

void timer1() interrupt 3
{
	TH1 = 0x3c;
	TL1 = 0xaf;//50ms开始计数
	num++;
	if(num == 10)//记到20，1s
	{
		flag1 = 1;
		num = 0;
		TR0 = 0;
		TR1 = 0;
	}
}