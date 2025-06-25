#include <reg52.h>
#include "oled.h"


void main() {
	OLED_Init();
	OLED_Clear();
	//OLED_ShowString(0,0,"*");
	//OLED_ShowString(1,0,"*");
	//OLED_ShowString(2,0,"*");
	//OLED_ShowString(3,0,"*");
	//OLED_ShowString(4,0,"*");
	//OLED_ShowString(5,0,"*");
//
	/*
	OLED_ShowString(6,0,"*");
	OLED_ShowString(7,0,"*");
	OLED_ShowString(8,0,"*");
	OLED_ShowString(9,0,"*");*/
	//OLED_ShowString(30,1,"*************");
//OLED_ShowString(30,2,"*************");
	/*
	OLED_ShowString(30,3,"*************");
	OLED_ShowString(30,4,"*************");
	OLED_ShowString(30,5,"*************");
	OLED_ShowString(30,6,"*************");
	OLED_ShowString(30,7,"*************");
	OLED_ShowString(30,8,"*************");*/
	OLED_ShowString(30,4,"Hello World!");
	OLED_ShowString(30,6,"WS !!!");

	while (1);
}