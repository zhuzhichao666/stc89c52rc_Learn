#include<reg51.h>

sbit button = P0^1;

void main()
{
	button = 1;//����������Ϊ����״̬
	while(1){
		if(button == 1)//�����ʾΪ��ƽΪ��
			P1 = 0XFF;//���������ʾ����
		else
			P1 = 0X00;//�����ʾLED����һ��Ϊ�͵�ƽ����
	}
}