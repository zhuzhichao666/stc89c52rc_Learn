#include <reg51.h>

unsigned char flag = 0;
unsigned char num = 0;
unsigned char flag1 = 0;
unsigned long int cnt = 0;
unsigned int pwm = 0;
unsigned int speed = 0;

unsigned char tab[] = {
    0xc0,  // 0
    0xf9,  // 1
    0xa4,  // 2
    0xb0,  // 3
    0x99,  // 4
    0x92,  // 5
    0x82,  // 6
    0xf8,  // 7
    0x80,  // 8
    0x90   // 9
};

void delay(unsigned int t)
{
	while(t--);
}

void main()
{
	TMOD = 0x15;//T0计数器T0定时器
	
	TH0 = 0;
	TL0 = 0;//计数器初值
	TH1 = 0x3c;
	TL1 = 0xb0;//定时器初值50ms
	
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
			pwm = TH0;
			pwm = pwm<<8;
			pwm = pwm|TL0;
			
			TH0 = 0;
			TL0 = 0;
			cnt = flag*65535+pwm;flag = 0;
			pwm = cnt/100;
			speed = pwm;
			
			TR0 = 1;
			TR1 = 1;
		} 
		P2 = 0x01;
		P0 =  tab[speed/1000];
		delay(500);
		
		P2 = 0x02;
		P0 =  tab[speed%1000/100];
		delay(500);
		
		P2 = 0x04;
		P0 =  tab[speed%100/10];
		delay(500);
		
		P2 = 0x08;
		P0 =  tab[speed%10];
		delay(500);
	}
}

void timer0() interrupt 1
{
	flag++;//计数器每65535+1
}

void timer1() interrupt 3
{
	TH1 = 0x3c;
	TL1 = 0xb0;//50ms开始计数
	num++;
	if(num == 20)//记到20，1s
	{
		flag1 = 1;
		num = 0;
		TR0 = 0;
		TR1 = 0;
	}
}