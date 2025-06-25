#include "oled.h"
#include "iic.h"
#include "ascii.h"

#define OLED_ADDRESS 0x78

void OLED_WriteCmd(unsigned char cmd) {
    IIC_Start();
    IIC_SendByte(OLED_ADDRESS);
    IIC_SendByte(0x00);
    IIC_SendByte(cmd);
    IIC_Stop();
}

void OLED_WriteData(unsigned char dat) {
    IIC_Start();
    IIC_SendByte(OLED_ADDRESS);
    IIC_SendByte(0x40);
    IIC_SendByte(dat);
    IIC_Stop();
}

void OLED_SetPos(unsigned char x, unsigned char y) {
    OLED_WriteCmd(0xB0 + y);
    OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);
    OLED_WriteCmd((x & 0x0F) | 0x01);
}

void OLED_Clear(void) {
		unsigned char x,y;
    for (y = 0; y < 8; y++) {
        OLED_SetPos(0, y);
        for (x = 0; x < 128; x++) {
            OLED_WriteData(0x00);
        }
    }
}

void OLED_ShowChar(unsigned char x, unsigned char y, char chr) {
    unsigned char c = chr - ' ';
		unsigned char i;
    OLED_SetPos(x, y);
    for (i = 0; i < 6; i++) {
        OLED_WriteData(AsciiTable[c][i]);
    }
}

void OLED_ShowString(unsigned char x, unsigned char y, char *str) {
    while (*str) {
        OLED_ShowChar(x, y, *str++);
        x += 6;
        if (x > 122) {
            x = 0;
            y++;
        }
    }
}

void OLED_Init(void) {
    OLED_WriteCmd(0xAE);
    OLED_WriteCmd(0x20); OLED_WriteCmd(0x10);
    OLED_WriteCmd(0xB0);
    OLED_WriteCmd(0xC8);
    OLED_WriteCmd(0x00); OLED_WriteCmd(0x10);
    OLED_WriteCmd(0x40);
    OLED_WriteCmd(0x81); OLED_WriteCmd(0xFF);
    OLED_WriteCmd(0xA1);
    OLED_WriteCmd(0xA6);
    OLED_WriteCmd(0xA8); OLED_WriteCmd(0x3F);
    OLED_WriteCmd(0xA4);
    OLED_WriteCmd(0xD3); OLED_WriteCmd(0x00);
    OLED_WriteCmd(0xD5); OLED_WriteCmd(0xF0);
    OLED_WriteCmd(0xD9); OLED_WriteCmd(0x22);
    OLED_WriteCmd(0xDA); OLED_WriteCmd(0x12);
    OLED_WriteCmd(0xDB); OLED_WriteCmd(0x20);
    OLED_WriteCmd(0x8D); OLED_WriteCmd(0x14);
    OLED_WriteCmd(0xAF);
    OLED_Clear();
}