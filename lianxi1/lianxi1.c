#include<reg51.h>

sbit button = P0^1;

void main()
{
	button = 1;//将引脚设置为输入状态
	while(1){
		if(button == 1)//这个表示为电平为高
			P1 = 0XFF;//所以这个表示不亮
		else
			P1 = 0X00;//这个表示LED灯另一端为低电平则亮
	}
}