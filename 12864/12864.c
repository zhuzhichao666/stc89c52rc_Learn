#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define DR(fre) (65536-(11059200/12)/(2*fre))  // Ƶ�ʻ���ʱ��ֵ

// Ӳ���ӿڶ���
sbit RS = P1^2;
sbit RW = P1^1;
sbit E  = P1^0;
sbit RESET = P1^3;
sbit PSB = P3^5;
sbit beep = P3^6;  // ������

#define LCD_DATA P2
#define KeyPort  P0

// ����Ƶ�ʱ�
uint code music[16] = {
	DR(262),DR(294),DR(330),DR(349),
	DR(392),DR(440),DR(494),DR(523),
	DR(587),DR(659),DR(698),DR(784),
	DR(880),DR(988),DR(1046),0
};

// ȫ�ֱ���
uchar key = 0xff;
uchar pre_key = 0xff;

// ��ʱ
void delay(uint ms)
{
	uint i;
	for(i=0; i<ms; i++)
	{
		TH1 = (65536-1000)>>8;
		TL1 = (65536-1000)&0xff;
		TR1 = 1;
		while(!TF1);
		TR1 = 0;
		TF1 = 0;
	}
}

// ���LCD�Ƿ�æ
bit check_busy()
{
	uint timeout = 5000;
	LCD_DATA = 0xFF;
	RS = 0;
	RW = 1;
	E = 1;
	while ((LCD_DATA & 0x80) && timeout--);
	E = 0;
	return (timeout > 0) ? 0 : 1;
}

// LCDд����
void write_cmd(uchar cmd)
{
	if (!check_busy())
	{
		RS = 0;
		RW = 0;
		LCD_DATA = cmd;
		E = 1;
		_nop_(); _nop_();
		E = 0;
		delay(5);
	}
}

// LCDд����
void write_data(uchar dat)
{
	if (!check_busy())
	{
		RS = 1;
		RW = 0;
		LCD_DATA = dat;
		E = 1;
		_nop_(); _nop_();
		E = 0;
		delay(5);
	}
}

// LCD��ʼ��
void lcd_init()
{
	PSB = 1;
	RESET = 0;
	delay(50);
	RESET = 1;
	delay(50);

	write_cmd(0x30);
	delay(10);
	write_cmd(0x0C);
	delay(10);
	write_cmd(0x01);
	delay(20);
	write_cmd(0x06);
	delay(10);
}

// ��ʾ�ַ���
void display_string(uchar x, uchar y, char *str)
{
	write_cmd(0x80 | (y << 6) | x);
	while (*str)
	{
		write_data(*str++);
	}
}

// ��ʾ����
void display_number(uchar x, uchar y, uint num)
{
	uchar buf[5];
	uchar i = 0;
	if (num == 0)
	{
		buf[0] = '0';
		i = 1;
	}
	else
	{
		while (num > 0 && i < 5)
		{
			buf[i++] = '0' + (num % 10);
			num /= 10;
		}
	}
	write_cmd(0x80 | (y << 6) | x);
	while (i > 0)
	{
		write_data(buf[--i]);
	}
}

// ɨ�谴��
uchar keyscan()
{
	uchar i, temp, keynum = 0xff;
	for(i=0; i<4; i++)
	{
		KeyPort = ~(0x01 << i);
		temp = KeyPort & 0xf0;
		if(temp != 0xf0)
		{
			delay(1);
			temp = KeyPort & 0xf0;
			if(temp != 0xf0)
			{
				if((temp & 0x10) == 0) keynum = i*4 + 0;
				else if((temp & 0x20) == 0) keynum = i*4 + 1;
				else if((temp & 0x40) == 0) keynum = i*4 + 2;
				else if((temp & 0x80) == 0) keynum = i*4 + 3;
			}
		}
	}
	return keynum;
}

// Timer0�жϣ����Ʒ�����������Ƶ�ʣ�
void timer0() interrupt 1
{
	TH0 = music[key] >> 8;
	TL0 = music[key] & 0xff;
	beep = ~beep;  // ��ת������
}

// ������
void main()
{
	TMOD = 0x11;  // Timer0��Timer1���÷�ʽ1
	EA = 1;
	ET0 = 1;  // ����Timer0�ж�

	lcd_init();
	display_string(0,0,"0:");	
	display_string(2,0,"1:");
	display_string(4,0,"2:");
	display_string(6,0,"3:");
	display_string(8,0,"4:");
	display_string(10,0,"5:");	
	display_string(12,0,"6:");
	display_string(14,0,"7:");
	display_string(16,0,"8:");
	display_string(18,0,"9:");
	display_string(20,0,"10:");
	display_string(22,0,"11:");
	display_string(24,0,"12:");
	display_string(26,0,"13:");
	display_string(28,0,"14:");
	display_string(30,0,"15:");
	while(1)
	{
		key = keyscan();
		if(key != 0xff)
		{
			if(key != pre_key)
			{
				pre_key = key;
				lcd_init();
				display_string(0,0,"key:");
				write_data('0' + (key/10));
				write_data('0' + (key%10));
				
				display_string(8,0,"Hz:");
				display_number(11, 0, (uint)((11059200/12) / (2 * (65536 - music[key])))); // ��ʾƵ��
				display_string(4,0,"2:");
				if (music[key] != 0)
				{
						TH0 = music[key] >> 8;
						TL0 = music[key] & 0xff;
						TR0 = 1;
				}
				else
				{
						TR0 = 0;
						beep = 1;
				}
			}
		}
		else
		{
			pre_key = 0xff;
			TR0 = 0;
			beep = 1;
		}
	}
}
